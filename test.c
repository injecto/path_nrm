#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "path_nrm.h"

void assert_normalization(char* expected, char* path) {
    char *result = normalize(path);

    if (!path && !result) {
        // both nulls, ok
        return;
    }

    if (strcmp(expected, result) != 0) {
        fprintf(stderr, "Normalization of `%s` failed: expected `%s`, but actual `%s`\n", path, expected, result);
    }

    free(result);
}

float frand() {
    return ((float) rand()) / RAND_MAX;
}

char* gen_part() {
    float p = frand();
    if (p <= 0.8) {
        return "x";
    } else if (p > 0.8 && p <= 0.9) {
        return "/";
    } else if (p > 0.9 && p <= 0.98) {
        return "/../";
    } else {
        return "/./";
    }
}

char* generate_path(size_t max_len) {
    char* path = malloc(max_len + 1);

    size_t generated = 0;
    char *part = gen_part();
    size_t part_len = strlen(part);
    for (; generated + part_len <= max_len; generated += part_len, part = gen_part(), part_len = strlen(part)) {
        strcpy(path + generated, part);
    }

    path[generated] = '\0';
    return path;
}

void performance_test(size_t path_len) {
    char* path = generate_path(path_len);

    clock_t start = clock();
    long i;
    char *result = normalize(path);

    clock_t consumed = clock() - start;
    double consumed_sec = ((double) consumed) / CLOCKS_PER_SEC;

    double speed = path_len / consumed_sec / (1000 * 1000 * 1000) * 8;   // gigabit
    printf("Normalization of %zu bytes string consumed %f seconds [%fGbps]\n", path_len, consumed_sec, speed);
    fflush(stdout);

    free(result);
}

int main() {
    assert_normalization(0, 0);
    assert_normalization("/", "");
    assert_normalization("/", "/");
    assert_normalization("/", ".");
    assert_normalization("/", "..");
    assert_normalization("/", "./.");
    assert_normalization("/", "./..");
    assert_normalization("/bin", "../bin");
    assert_normalization("/bin", "../../bin");
    assert_normalization("/bin/bash", "/bin/bash");
    assert_normalization("/var/local", "/var/lib/../local");
    assert_normalization("/var/lib/local", "/var/lib/./local");
    assert_normalization("/local", "/var/../../local");
    assert_normalization("/usr", "home/root/.././user/../../../usr/.");

    size_t chunk_sizes[] = {128, 1024, 1024 * 1024, 128 * 1024 * 1024, 512 * 1024 * 1024};
    for (int i = 0; i < sizeof(chunk_sizes)/sizeof(*chunk_sizes); ++i) {
        performance_test(chunk_sizes[i]);
    }

    return 0;
}

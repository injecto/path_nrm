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
        fprintf(stderr, "Normalization of `%s` failed: expected `%s`, but actual `%s`", path, expected, result);
        exit(1);
    }

    free(result);
}

float frand() {
    return ((float) rand()) / RAND_MAX;
}

char* gen_part() {
    float p = frand();
    if (p <= 0.5) {
        return "x";
    } else if (p > 0.5 && p <= 0.7) {
        return "/";
    } else if (p > 0.7 && p <= 0.9) {
        return "../";
    } else {
        return "./";
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

void performance_test(size_t path_len, size_t iter_num) {
    char* path = generate_path(path_len);
    char* results[iter_num];

    clock_t start = clock();
    long i;
    for (i = 0; i < iter_num; ++i) {
        results[i] = normalize(path);
    }

    clock_t consumed = clock() - start;
    double consumed_sec = ((double) consumed) / CLOCKS_PER_SEC;

    size_t len = strlen(path);
    double speed = len * iter_num / consumed_sec / (1024 * 1024 * 1024);
    printf("Normalization of %zu bytes string %zu times consumed %f seconds [%fGbps]\n", len, iter_num, consumed_sec, speed);
    fflush(stdout);

    for (i = 0; i < iter_num; ++i) {
        free(results[i]);
    }
}

int main() {
    assert_normalization(0, 0);
    assert_normalization("", "");

    assert_normalization("/", "/");
    assert_normalization("", ".");
    assert_normalization("/", "..");
    assert_normalization("", "./.");
    assert_normalization("/", "./..");

    assert_normalization("/bin", "../bin");
    assert_normalization("/bin", "../../bin");
    assert_normalization("/bin/bash", "/bin/bash");
    assert_normalization("/bin/bash", "/bin//bash");
    assert_normalization("/var/local", "/var/lib/../local");
    assert_normalization("/var/lib/local", "/var/lib/./local");
    assert_normalization("/local", "/var/../../local");

    size_t lengths[] = {   1024,   2048, 32 * 1024 * 1024, 48 * 1024 * 1024, 128 * 1024 * 1024, 256 * 1024 * 1024 };
    size_t iters[] =   { 100000, 100000,              100,              100,               100,               100 };
    for (int i = 0; i < sizeof(lengths)/sizeof(*lengths); ++i) {
        performance_test(lengths[i], iters[i]);
    }

    return 0;
}

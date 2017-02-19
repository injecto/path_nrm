#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static const int DELIMITER = '/';
static const char* ROOT = "/";
static const char* SELF = ".";
static const char* SELF_D = "./";
static const char* PARENT = "..";
static const char* PARENT_D = "../";

struct builder {
    char* path;             // current path state
    size_t path_top;        // current write position of path array
    size_t* parts_len;      // stack of append parts lengths
    size_t parts_len_top;   // current lengths stack top
};

/**
 * Create builder
 * @param len Size of buffer
 * @return Builder
 */
struct builder create_builder(size_t len) {
    struct builder b;
    b.path = malloc(len + 1);
    b.path_top = 0;
    b.parts_len = malloc(len * sizeof(size_t));
    b.parts_len_top = 0;
    return b;
}

/**
 * Append new path part
 * @param b Builder
 * @param part Part
 * @param len Part length
 */
void append(struct builder* b, const char *part, size_t len) {
    if (b->path_top != 0 && strncmp(ROOT, part, len) == 0) {
        // skip redundant slash
        return;
    }

    strncpy(b->path + b->path_top, part, len);
    b->path_top += len;
    b->parts_len[b->parts_len_top] = len;
    b->parts_len_top++;
}

/**
 * Drop latest added part
 * @param b Builder
 */
void drop_latest(struct builder* b) {
    if (b->parts_len_top == 0) {
        // we have reached a root, so make it explicit
        append(b, ROOT, strlen(ROOT));
        return;
    }

    size_t new_top = b->parts_len_top - 1;
    size_t latest_len = b->parts_len[new_top];

    if (strncmp(ROOT, b->path + b->path_top - latest_len, latest_len) == 0) {
        // don't drop the root
        return;
    }

    b->path_top -= latest_len;
    b->parts_len_top = new_top;
}

/**
 * Build result path
 * @param b Builder
 * @return Heap allocated normalized path string
 */
char* build(struct builder* b) {
    free(b->parts_len);
    b->path[b->path_top] = '\0';
    return b->path;
}

void next_part(struct builder* b, const char* p, size_t len) {
    if (len == 0) {
        return;
    }

    if (strncmp(SELF_D, p, len) == 0 || strncmp(SELF, p, len) == 0) {
        return;
    }

    if (strncmp(PARENT_D, p, len) == 0 || strncmp(PARENT, p, len) == 0) {
        drop_latest(b);
    } else {
        append(b, p, len);
    }
}

char *normalize(const char *path) {
    if (!path)
        return 0;

    struct builder b = create_builder(strlen(path));
    const char *part_start = path;
    const char *part_end = strchr(path, DELIMITER);

    while (part_end != 0) {
        next_part(&b, part_start, part_end + 1 - part_start);

        part_start = part_end + 1;
        part_end = strchr(part_start, DELIMITER);
    }
    next_part(&b, part_start, strlen(part_start));

    return build(&b);
}

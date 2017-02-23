#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *copy(char *dest, const char *src, const char *until) {
    unsigned long l = until - src;
    if (dest != src) {
        strncpy(dest, src, l);
    }
    return dest + l;
}

char *normalize(const char *path) {
    if (!path)
        return 0;

    bool root_needed = *path != '/';

    size_t len = strlen(path);
    if (root_needed) ++len;

    char* normalized = (char*) malloc(len+1);
    if (root_needed) {
        *normalized = '/';
        strcpy(normalized+1, path);
    } else {
        strcpy(normalized, path);
    }

    char* w = normalized;       // write position
    const char* segment = w;    // current segment

    char* slash_dot = strstr(normalized, "/."); // special token
    while (slash_dot) {
        if (*(slash_dot + 2) == '.' && (*(slash_dot + 3) == '/' || *(slash_dot + 3) == '\0')) {
            // '/..': copy, rewind and skip
            w = copy(w, segment, slash_dot);
            if (w-1 >= normalized) while (*(--w) != '/');
            segment = slash_dot + 3;
        } else if (*(slash_dot + 2) == '/' || *(slash_dot + 2) == '\0') {
            // '/.': copy and skip
            w = copy(w, segment, slash_dot);
            segment = slash_dot + 2;
        }
        slash_dot = strstr(segment, "/.");
    }

    strcpy(w, segment);
    if (*normalized == '\0') strcpy(normalized, "/");   // corner case

    return normalized;
}

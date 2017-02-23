#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char* rewind(char* pos, char* limit) {
    do {
        if (pos == limit) {
            *pos = '/';
            return pos + 1;
        }
        --pos;
    } while (*pos != '/');
    return pos;
}

char *process(char *result, char *w, char *r, char *p) {
    unsigned long l = r - p;
    if (l != 0) {
        if (strncmp("/..", p, 3) == 0 || strncmp("..", p, 2) == 0) {
            w = rewind(w, result);
        } else if (strncmp("/.", p, 2) != 0 && *p != '.') {
            if (w - 1 >= result && *(w - 1) == '/') {
                w--;
            }
            if (w != p) {
                strncpy(w, p, l);
            }
            w += l;
        }
    }
    return w;
}

char *normalize(const char *path) {
    if (!path)
        return 0;

    size_t len = strlen(path);
    char* result = (char*) malloc(len + 1);
    strcpy(result, path);

    char* w = result;
    char* r = w;
    char* p = w;

    while (*r) {
        if (*r == '/') {
            w = process(result, w, r, p);
            p = r;
        }

        r++;
    }

    w = process(result, w, r, p);
    *w = '\0';
    return result;
}

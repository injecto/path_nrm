#ifndef PATH_NRM_PATH_NRM_H
#define PATH_NRM_PATH_NRM_H

/**
 * Normalize unix-style path.<br/>
 * Function is thread safe and compute result in O(n).
 *
 * @param path ASCII encoded path
 * @return Normalized path, allocated on heap. So it need to be free after use.
 */
char *normalize(const char *path);

#endif //PATH_NRM_PATH_NRM_H

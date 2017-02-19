# Path normalizer

Helper function that _normalizes_ strings containing filesystem paths. 
The file system is assumed to be unix, with slash-separated components.

This function is going to be somewhat specialized for the webserver, so `domain.com/../foo` is expected to be equivalent to `domain.com/foo`.

Examples of path normalization:

| Path              | Normalized        |
|-------------------|-------------------|
| `../bar`          | `/bar`            |
| `/foo/bar`        | `/foo/bar`        |
| `/foo/bar/../baz` | `/foo/baz`        |
| `/foo/bar/./baz/` | `/foo/bar/baz/`   |
| `/foo/../../baz`  | `/baz`            |

## Build

```
cmake -DCMAKE_BUILD_TYPE=Release
make
```
So you've got static library `libpath_nrm.a` and executable of tests `path_nrm_test`.

## Implementation notes

- ASCII encoded strings allowed only.
- No external dependencies.
- Computing complexity *O(n)*, memory consumption *O(n)*.
- Thread safe.

## Performance

Test suite: Intel Core i3 4160 (3.6GHz, 64K/256K/3072K caches), 16Gb RAM (no swap), Ubuntu 16.04 x64.

```
Normalization of 1024 bytes string 100000 times consumed 2.670622 seconds [0.035710Gbps]
Normalization of 2048 bytes string 100000 times consumed 5.186243 seconds [0.036777Gbps]
Normalization of 33554432 bytes string 100 times consumed 88.122649 seconds [0.035462Gbps]
Normalization of 50331647 bytes string 100 times consumed 132.354386 seconds [0.035416Gbps]
Normalization of 134217726 bytes string 100 times consumed 355.552843 seconds [0.035157Gbps]
Normalization of 268435454 bytes string 100 times consumed 723.847981 seconds [0.034538Gbps]
```
As can be seen from the above normalization speed is about 0.035Gbps per core.

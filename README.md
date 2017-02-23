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
Normalization of 128 bytes string consumed 0.000001 seconds [1.024000Gbps]
Normalization of 1024 bytes string consumed 0.000003 seconds [2.730667Gbps]
Normalization of 1048576 bytes string consumed 0.003299 seconds [2.542773Gbps]
Normalization of 134217728 bytes string consumed 0.343688 seconds [3.124176Gbps]
Normalization of 536870912 bytes string consumed 1.719848 seconds [2.497295Gbps]
```

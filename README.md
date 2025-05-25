# SIMD-optimized Lattice Transformations

## Build

```bash
mkdir -p subprojects
meson wrap install gtest
meson wrap install google-benchmark
meson setup build
meson compile -C build
```

## Notes

* Splitting up headers into headers/source significantly impacts performance unless LTO is enabled (`-flto`).
  * For now keeping things header-only.

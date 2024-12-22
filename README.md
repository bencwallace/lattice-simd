# SIMD-optimized Lattice Transformations

## Notes

* Splitting up headers into headers/source significantly impacts performance unless LTO is enabled (`-flto`).
  * For now keeping things header-only.

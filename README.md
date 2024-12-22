# SIMD Lattice Transformations

Todo:

* Transform point
* Transform box
* Compose transforms
* Box intersection
* Box union
* Point arithmetic
* Translate box

Ideas:

* Transform multiple coordinates simultaneously
* Transform multiple points simultaneously
* Transform boxes

## Notes

* Splitting up headers into headers/source significantly impacts performance unless LTO is enabled (`-flto`).
  * For now keeping things header-only.

# References

* https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX2
* https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html

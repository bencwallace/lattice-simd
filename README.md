# SIMD Lattice Transformations

## Ideas

**Transform multiple coordinates simultaneously**

* `(uint64_t, uint64_t)`
  * Interpreted as 2 large coordinates
* `(uint32_t, uint32_t, uint32_t, uint32_t)`
  * Can be interpreted as 4 coordinates or 2-3 coordinates if some values are ignored -- what are the performance implications of doing this?

**Transform multiple points simultaneously**

**Transform boxes**

# References

* https://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX2
* https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html

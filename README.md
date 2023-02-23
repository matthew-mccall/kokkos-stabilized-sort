# Kokkos Stabilized Unstable Sort
When sorting elements in an array, an unstable sorting algorithm may not always preserve the order of equal elements, resulting in different orderings each time the algorithm is run on the same array. To stabilize the sorting algorithm, a permutation array can be used.

This repository contains examples of a stable sorting algorithm, an unstable sorting algorithm, and a stabilized unstable sorting algorithm. 
- `std_stable_sort` contains an example of a stable sorting algorithm using `std::stable_sort`. 
- `kokkos_unstable_sort` contains an example of an unstable sorting algorithm using `Kokkos::Experimental::sort_team`. 
- `kokkos_stabilized_unstable_sort` contains an example of a stabilized unstable sorting algorithm using `Kokkos::Experimental::sort_team` and a permutation array.

## Usage
`./KokkosStabilizedUnstableSort [number_of_pairs] [number_of_unique_values_per_key]]`

Each example takes the total number of pairs and the number of unique values per key.
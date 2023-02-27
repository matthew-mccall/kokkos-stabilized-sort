cmake_minimum_required(VERSION 3.20)
project(kokkos_pair_sort)

include(CTest)

add_subdirectory(libs/kokkos)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(kokkos_stabilized_unstable_sort)
add_subdirectory(kokkos_old_sort_int)
add_subdirectory(kokkos_unstable_sort)
add_subdirectory(kokkos_unstable_sort_int)
add_subdirectory(std_stable_sort)

add_test(NAME kokkos_stabilized_unstable_sort COMMAND KokkosStabilizedUnstableSort 30 3)
add_test(NAME kokkos_old_sort_int COMMAND KokkosOldSortInt 30)
add_test(NAME kokkos_unstable_sort COMMAND KokkosUnstableSort 30 3)
add_test(NAME kokkos_unstable_sort_int COMMAND KokkosUnstableSortInt 30)
add_test(NAME std_stable_sort COMMAND StdStableSort 30 3)
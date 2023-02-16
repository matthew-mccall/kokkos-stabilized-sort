cmake_minimum_required(VERSION 3.24)
project(kokkos_pair_sort)

include(CTest)

add_subdirectory(libs/kokkos)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(kokkos_unstable_sort)

add_test(NAME kokkos_unstable_sort_pair_int_int COMMAND UnstableSortPairIntInt)
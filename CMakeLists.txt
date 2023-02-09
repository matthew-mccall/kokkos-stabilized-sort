cmake_minimum_required(VERSION 3.24)
project(kokkos_pair_sort)

add_subdirectory(libs/kokkos)

set(CMAKE_CXX_STANDARD 17)

add_executable(kokkos_pair_sort main.cpp)
target_link_libraries(kokkos_pair_sort Kokkos::kokkos)

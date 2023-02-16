cmake_minimum_required(VERSION 3.24)
project(UnstableSortPairIntInt)

add_executable(UnstableSortPairIntInt main.cpp)
target_link_libraries(UnstableSortPairIntInt Kokkos::kokkos)
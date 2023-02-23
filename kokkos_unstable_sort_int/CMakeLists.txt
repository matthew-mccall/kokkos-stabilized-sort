cmake_minimum_required(VERSION 3.20)
project(KokkosUnstableSortInt)

add_executable(KokkosUnstableSortInt main.cpp)
target_link_libraries(KokkosUnstableSortInt Kokkos::kokkos)
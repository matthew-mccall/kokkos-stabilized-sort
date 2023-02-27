cmake_minimum_required(VERSION 3.20)
project(KokkosOldSortInt)

add_executable(KokkosOldSortInt main.cpp)
target_link_libraries(KokkosOldSortInt Kokkos::kokkos)
cmake_minimum_required(VERSION 3.24)
project(KokkosUnstableSort)

add_executable(KokkosUnstableSort main.cpp)
target_link_libraries(KokkosUnstableSort Kokkos::kokkos)
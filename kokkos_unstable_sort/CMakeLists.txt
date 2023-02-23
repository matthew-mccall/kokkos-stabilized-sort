cmake_minimum_required(VERSION 3.20)
project(KokkosUnstableSort)

add_executable(KokkosUnstableSort main.cpp)
target_link_libraries(KokkosUnstableSort Kokkos::kokkos)
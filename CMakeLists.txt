cmake_minimum_required(VERSION 3.20)
project(kokkos_pair_sort)

include(CTest)

add_subdirectory(libs/kokkos)

if (ENABLE_KOKKOS_TOOLS)
	add_subdirectory(libs/kokkos-tools)
endif ()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(kokkos_stabilized_unstable_sort)
add_subdirectory(kokkos_old_sort_int)
add_subdirectory(kokkos_unstable_sort)
add_subdirectory(kokkos_unstable_sort_int)
add_subdirectory(std_stable_sort)

file(GLOB KOKKOS_OLD_TEST_RESULTS ${CMAKE_CURRENT_BINARY_DIR}/*.dat)
if (KOKKOS_OLD_TEST_RESULTS)
    file(REMOVE ${KOKKOS_OLD_TEST_RESULTS})
endif()

add_test(NAME kokkos_stabilized_unstable_sort COMMAND KokkosStabilizedUnstableSort 1000 10)
add_test(NAME kokkos_old_sort_int COMMAND KokkosOldSortInt 1000)
add_test(NAME kokkos_unstable_sort COMMAND KokkosUnstableSort 1000 10)
add_test(NAME kokkos_unstable_sort_int COMMAND KokkosUnstableSortInt 1000)
add_test(NAME std_stable_sort COMMAND StdStableSort 1000 10)

if (ENABLE_KOKKOS_TOOLS)
set_tests_properties(
        kokkos_stabilized_unstable_sort
        kokkos_old_sort_int
        kokkos_unstable_sort
        kokkos_unstable_sort_int
        std_stable_sort
        PROPERTIES ENVIRONMENT "KOKKOS_TOOLS_LIBS=${CMAKE_CURRENT_BINARY_DIR}/libs/kokkos-tools/profiling/simple-kernel-timer/${CMAKE_SHARED_LIBRARY_PREFIX}kp_kernel_timer${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif ()

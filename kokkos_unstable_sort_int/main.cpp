//
// Created by Matthew McCall on 2/16/23.
//
#include "Kokkos_Core.hpp"
#include "Kokkos_NestedSort.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>

// function for printing a view of pairs
template <typename ViewType>
void print_view(ViewType view) {
    // Deep copy to host
    Kokkos::View<std::uint32_t *, Kokkos::HostSpace> host_view("host_view", view.extent(0));
    Kokkos::deep_copy(host_view, view);

    // Print deep copy
    for (std::uint32_t i = 0; i < view.extent(0); i++) {
        std::cout << host_view(i);

        if (i < view.extent(0) - 1) {
            std::cout << ", ";
        } else {
            std::cout << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    assert(argc == 2);

    unsigned long NUMBER_OF_INTS = std::stoul(std::string {argv[1]});

    std::cout << "Using " << NUMBER_OF_INTS << std::endl;
    
    Kokkos::ScopeGuard kokkos;

    // Kokkos generate array of ints
    Kokkos::View<std::uint32_t*> b("b", NUMBER_OF_INTS);
    Kokkos::parallel_for("Init", NUMBER_OF_INTS, KOKKOS_LAMBDA (const std::uint32_t i) {
        b(i) = NUMBER_OF_INTS - 1 - i;
    });

    std::cout << "Before sort" << std::endl;
    print_view(b);

    //=== Kokkos nested sort by key with sort_team ===

    // Create a team policy
    Kokkos::TeamPolicy<> policy(NUMBER_OF_INTS, Kokkos::AUTO());

    //=== Kokkos nested sort with custom comparator ===
    Kokkos::parallel_for("Sort", policy, KOKKOS_LAMBDA (const Kokkos::TeamPolicy<>::member_type& team) {
        Kokkos::Experimental::sort_team(team, b);
    });

    // Print pairs
    std::cout << "After sort" << std::endl;
    print_view(b);

    return 0;
}

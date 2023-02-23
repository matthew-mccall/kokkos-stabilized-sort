//
// Created by Matthew McCall on 2/16/23.
//
#include "Kokkos_Core.hpp"
#include "Kokkos_NestedSort.hpp"

#include <cstdint>
#include <iostream>

// function for printing a view of pairs
template <typename ViewType>
void print_view(ViewType view) {
    // Deep copy to host
    Kokkos::View<Kokkos::pair<std::uint32_t, std::uint32_t>*, Kokkos::HostSpace> host_view("host_view", view.extent(0));
    Kokkos::deep_copy(host_view, view);

    // Print deep copy
    for (std::uint32_t i = 0; i < view.extent(0); i++) {
        std::cout << '(' << host_view(i).first << " " << host_view(i).second << ')';

        if (i < view.extent(0) - 1) {
            std::cout << ", ";
        } else {
            std::cout << std::endl;
        }
    }
}

// Custom comparator for sorting
struct custom_comparator {
    KOKKOS_INLINE_FUNCTION
    bool operator()(const Kokkos::pair<std::uint32_t, std::uint32_t>& a, const Kokkos::pair<std::uint32_t, std::uint32_t>& b) const {
        return a.first < b.first;
    }
};

int main(int argc, char **argv) {
    assert(argc == 3);

    unsigned long NUMBER_OF_PAIRS = std::stoul(std::string {argv[1]});
    unsigned long PAIR_GROUPS = std::stoul(std::string {argv[2]});

    std::cout << "Using " << NUMBER_OF_PAIRS << " pairs with " << PAIR_GROUPS << " unique values per key" << std::endl;
    
    Kokkos::ScopeGuard kokkos;

    // Kokkos generate array of pairs
    Kokkos::View<Kokkos::pair<std::uint32_t, std::uint32_t>*> b("b", NUMBER_OF_PAIRS);
    Kokkos::parallel_for("Init", NUMBER_OF_PAIRS, KOKKOS_LAMBDA (const std::uint32_t i) {
        b(i) = Kokkos::pair<std::uint32_t, std::uint32_t>((NUMBER_OF_PAIRS - 1 - i) / PAIR_GROUPS, (i % PAIR_GROUPS) + 1);
    });

    std::cout << "Before sort" << std::endl;
    print_view(b);

    //=== Kokkos nested sort by key with sort_team ===

    // Create a team policy
    Kokkos::TeamPolicy<> policy(NUMBER_OF_PAIRS, Kokkos::AUTO());

    //=== Kokkos nested sort with custom comparator ===
    Kokkos::parallel_for("Sort", policy, KOKKOS_LAMBDA (const Kokkos::TeamPolicy<>::member_type& team) {
        Kokkos::Experimental::sort_team(team, b, custom_comparator());
    });

    // Print pairs
    std::cout << "After sort" << std::endl;
    print_view(b);

    return 0;
}

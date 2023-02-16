//
// Created by Matthew McCall on 2/16/23.
//
#include "Kokkos_Core.hpp"
#include "Kokkos_NestedSort.hpp"

#include <iostream>

// function for printing a view of pairs
template <typename ViewType>
void print_view(ViewType view) {
    for (int i = 0; i < view.extent(0); i++) {
        std::cout << view(i).first << " " << view(i).second << std::endl;
    }
}

// Custom comparator for sorting
struct custom_comparator {
    KOKKOS_INLINE_FUNCTION
    bool operator()(const Kokkos::pair<int, int>& a, const Kokkos::pair<int, int>& b) const {
        return a.first < b.first;
    }
};

int main() {
    Kokkos::initialize();

    // Kokkos generate array of pairs
    Kokkos::View<Kokkos::pair<int, int>*> b("b", 10);
    Kokkos::parallel_for("Init", 10, KOKKOS_LAMBDA (const int i) {
        b(i) = Kokkos::pair<int, int>(10 - i, i);
    });

    std::cout << "Before sort" << std::endl;
    print_view(b);

    //=== Kokkos nested sort by key with sort_team ===

    // Create a team policy
    Kokkos::TeamPolicy<> policy(10, 1);

    //=== Kokkos nested sort with custom comparator ===
    Kokkos::parallel_for("Sort", policy, KOKKOS_LAMBDA (const Kokkos::TeamPolicy<>::member_type& team) {
        Kokkos::Experimental::sort_team(team, b, custom_comparator());
    });

    // Print pairs
    std::cout << "After sort" << std::endl;
    print_view(b);

    return 0;
}

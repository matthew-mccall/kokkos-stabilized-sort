//
// Created by Matthew McCall on 2/16/23.
//

#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

#include "Kokkos_Core.hpp"
#include "Kokkos_NestedSort.hpp"

template<typename T>
void print_vector(std::vector<std::pair<std::uint32_t, std::uint32_t>> &a, Kokkos::View<T*> view) {
    // Deep copy to host
    Kokkos::View<T*, Kokkos::HostSpace> host_view("host_view", view.extent(0));
    Kokkos::deep_copy(host_view, view);

    // Print deep copy
    for (std::uint32_t i = 0; i < view.extent(0); i++) {
        std::cout << '(' << a[host_view(i)].first << " " << a[host_view(i)].second << ')';

        if (i < view.extent(0) - 1) {
            std::cout << ", ";
        } else {
            std::cout << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    constexpr std::uint32_t NUMBER_OF_PAIRS = 30;
    constexpr std::uint32_t PAIR_GROUPS = 3;

    Kokkos::ScopeGuard kokkos;

    std::vector<std::pair<std::uint32_t, std::uint32_t>> a(NUMBER_OF_PAIRS);

    for (std::uint32_t i = 0; i < NUMBER_OF_PAIRS; i++) {
        a[i] = std::pair<std::uint32_t, std::uint32_t>((NUMBER_OF_PAIRS - 1 - i) / PAIR_GROUPS, (i % PAIR_GROUPS) + 1);
    }

    // Kokkos generate index array
    Kokkos::View<std::uint32_t*> indices("b", NUMBER_OF_PAIRS); // also known as a permutation array
    Kokkos::parallel_for("Init", NUMBER_OF_PAIRS, KOKKOS_LAMBDA (const std::uint32_t i) {
        indices(i) = i;
    });

    std::cout << "Before sort" << std::endl;
    print_vector(a, indices);

    auto functor = KOKKOS_LAMBDA (const Kokkos::TeamPolicy<>::member_type& team) {
        Kokkos::Experimental::sort_team(team, indices, [&a](std::uint32_t i, std::uint32_t j) {
            // compare pairs by key, the by index
            if (a[i].first == a[j].first) {
                // Since the indices are sorted, we can just compare them
                return i < j;
            }

            return a[i].first < a[j].first;
        });
    };

    Kokkos::TeamPolicy<> policy(NUMBER_OF_PAIRS, Kokkos::AUTO());

    //=== Kokkos nested sort with custom comparator ===
    Kokkos::parallel_for("Sort", policy, KOKKOS_LAMBDA (const Kokkos::TeamPolicy<>::member_type& team) {
        Kokkos::Experimental::sort_team(team, indices, [&a](std::uint32_t i, std::uint32_t j) {
            // compare pairs by key, the by index
            if (a[i].first == a[j].first) {
                // Since the indices are sorted, we can just compare them
                return i < j;
            }

            return a[i].first < a[j].first;
        });
    });

    std::cout << "After sort" << std::endl;
    print_vector(a, indices);
}

//
// Created by Matthew McCall on 2/16/23.
//

#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

#include "Kokkos_Core.hpp"
#include "Kokkos_NestedSort.hpp"

template<typename T, typename U>
void print_vector(Kokkos::View<T*> a, Kokkos::View<U*> indices) {
    // deep copy to host
    Kokkos::View<T*, Kokkos::HostSpace> host_a("host_a", a.extent(0));
    Kokkos::View<U*, Kokkos::HostSpace> host_indices("host_indices", indices.extent(0));

    Kokkos::deep_copy(host_a, a);
    Kokkos::deep_copy(host_indices, indices);

    // print deep copy
    for (std::uint32_t i = 0; i < a.extent(0); i++) {
        std::cout << '(' << host_a[host_indices[i]].first << " " << host_a[host_indices[i]].second << ')';

        if (i < a.extent(0) - 1) {
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

    Kokkos::View<Kokkos::pair<std::uint32_t, std::uint32_t>*> a("a", NUMBER_OF_PAIRS);
    Kokkos::parallel_for("Init", NUMBER_OF_PAIRS, KOKKOS_LAMBDA (const std::uint32_t i) {
        a(i) = Kokkos::pair<std::uint32_t, std::uint32_t>((NUMBER_OF_PAIRS - 1 - i) / PAIR_GROUPS, (i % PAIR_GROUPS) + 1);
    });

    // Kokkos generate index array
    Kokkos::View<std::uint32_t*> indices("b", NUMBER_OF_PAIRS); // also known as a permutation array
    Kokkos::parallel_for("Init", NUMBER_OF_PAIRS, KOKKOS_LAMBDA (const std::uint32_t i) {
        indices(i) = i;
    });

    std::cout << "Before sort" << std::endl;
    print_vector(a, indices);

    Kokkos::TeamPolicy<> policy(NUMBER_OF_PAIRS, Kokkos::AUTO());

    //=== Kokkos nested sort with custom comparator ===
    Kokkos::parallel_for("Sort", policy, KOKKOS_LAMBDA (const Kokkos::TeamPolicy<>::member_type& team) {
        Kokkos::Experimental::sort_team(team, indices, [a](std::uint32_t i, std::uint32_t j) {
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

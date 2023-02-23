//
// Created by Matthew McCall on 2/16/23.
//

#include <iostream>
#include <utility>
#include <vector>

#include "Kokkos_Core.hpp"
#include "Kokkos_NestedSort.hpp"

template<typename T>
void print_vector(std::vector<std::pair<int, int>> &a, Kokkos::View<T*> view) {
    // Deep copy to host
    Kokkos::View<T*, Kokkos::HostSpace> host_view("host_view", view.extent(0));
    Kokkos::deep_copy(host_view, view);

    // Print deep copy
    for (int i = 0; i < view.extent(0); i++) {
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
    Kokkos::ScopeGuard kokkos;

    std::vector<std::pair<int, int>> a(30);

    for (int i = 0; i < 30; i++) {
        a[i] = std::pair<int, int>((29 - i) / 3, (i % 3) + 1);
    }


    // Kokkos generate index array
    Kokkos::View<int*> indices("b", 30); // also known as a permutation array
    Kokkos::parallel_for("Init", 30, KOKKOS_LAMBDA (const int i) {
        indices(i) = i;
    });

    std::cout << "Before sort" << std::endl;
    print_vector(a, indices);

    Kokkos::TeamPolicy<> policy(30, 1);

    //=== Kokkos nested sort with custom comparator ===
    Kokkos::parallel_for("Sort", policy, KOKKOS_LAMBDA (const Kokkos::TeamPolicy<>::member_type& team) {
        Kokkos::Experimental::sort_team(team, indices, [&a](int i, int j) {
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

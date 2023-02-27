//
// Created by Matthew McCall on 2/27/23.
//
#include "Kokkos_Core.hpp"
#include "Kokkos_Sort.hpp"

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

    // Kokkos sort
    Kokkos::sort(b);

    // Print pairs
    std::cout << "After sort" << std::endl;
    print_view(b);

    return 0;
}
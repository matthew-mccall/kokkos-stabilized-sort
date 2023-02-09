#include "Kokkos_Core.hpp"
#include "Kokkos_Sort.hpp"

#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;

    // Simple kokkos for each
    Kokkos::parallel_for("HelloWorld", 10, KOKKOS_LAMBDA (const int i) {
        std::cout << "Hello, World! " << i << std::endl;
    });

    // Simple kokkos sort
    Kokkos::View<int*> a("a", 10);
    Kokkos::parallel_for("Init", 10, KOKKOS_LAMBDA (const int i) {
        a(i) = 10 - i;
    });

    Kokkos::sort(a);

    // Kokkos array of pairs
    Kokkos::View<Kokkos::pair<int, int>*> b("b", 10);

    // Sort b with Kokkos::Experimental::sort

    return 0;
}

//
// Created by Matthew McCall on 2/16/23.
//

#include <algorithm>
#include <cstdint>
#include <execution>
#include <iostream>
#include <utility>
#include <vector>

void print_vector(std::vector<std::pair<std::uint32_t, std::uint32_t>> a) {
    for (std::uint32_t i = 0; i < a.size(); i++) {
        std::cout << '(' << a[i].first << " " << a[i].second << ')';

        if (i < a.size() - 1) {
            std::cout << ", ";
        } else {
            std::cout << std::endl;
        }
    }
}

struct custom_comparator {
    bool operator()(const std::pair<std::uint32_t, std::uint32_t>& a, const std::pair<std::uint32_t, std::uint32_t>& b) const {
        return a.first < b.first;
    }
};

int main(int argc, char **argv)
{
    constexpr std::uint32_t NUMBER_OF_PAIRS = 30;
    constexpr std::uint32_t PAIR_GROUPS = 3;
    
    std::vector<std::pair<std::uint32_t, std::uint32_t>> a(NUMBER_OF_PAIRS);

    for (std::uint32_t i = 0; i < NUMBER_OF_PAIRS; i++) {
        a[i] = std::pair<std::uint32_t, std::uint32_t>((NUMBER_OF_PAIRS - 1 - i) / PAIR_GROUPS, (i %  PAIR_GROUPS) + 1);
    }

    std::cout << "Before sort" << std::endl;
    print_vector(a);

    std::stable_sort(std::execution::par_unseq, a.begin(), a.end());

    std::cout << "After sort" << std::endl;
    print_vector(a);
}

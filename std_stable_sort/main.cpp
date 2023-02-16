//
// Created by Matthew McCall on 2/16/23.
//

#include <algorithm>
#include <execution>
#include <iostream>
#include <utility>
#include <vector>

void print_vector(std::vector<std::pair<int, int>> a) {
    for (int i = 0; i < a.size(); i++) {
        std::cout << '(' << a[i].first << " " << a[i].second << ')';

        if (i < a.size() - 1) {
            std::cout << ", ";
        } else {
            std::cout << std::endl;
        }
    }
}

struct custom_comparator {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
        return a.first < b.first;
    }
};

int main(int argc, char **argv)
{
    std::vector<std::pair<int, int>> a(30);

    for (int i = 0; i < 30; i++) {
        a[i] = std::pair<int, int>((29 - i) / 3, (i % 3) + 1);
    }

    std::cout << "Before sort" << std::endl;
    print_vector(a);

    std::stable_sort(std::execution::par_unseq, a.begin(), a.end());

    std::cout << "After sort" << std::endl;
    print_vector(a);
}

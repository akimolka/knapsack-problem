#include "bigint.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

// number types  ------

using BigInteger = long long;
using Rational = long double;

// number types  ------


struct Item {
    BigInteger weight;
    BigInteger cost;
};

void relax(BigInteger& old_cost, const BigInteger& new_cost) {
    if (old_cost < new_cost) // if old_cost was -1, will update correctly
        old_cost = new_cost;
}

void update(std::vector<BigInteger>& best_cost, const Item& new_item) {
    BigInteger max_weight = BigInteger(best_cost.size()) - 1;
    for (int w = max_weight - new_item.weight; w >= 0; --w)
        if (best_cost[w] != -1)
            relax(best_cost[w + new_item.weight], best_cost[w] + new_item.cost);
}


BigInteger get_optimal_cost(
        int n,
        BigInteger max_weight,
        const std::vector<Item>& items) {

    std::vector<BigInteger> best_cost(max_weight + 1, -1);
    best_cost[0] = 0;

    for (int i = 0; i < n; ++i)
        update(best_cost, items[i]);
    return *std::max_element(best_cost.begin(), best_cost.end());
}

int main() {
    int n;
    BigInteger max_weight;
    std::cin >> n >> max_weight;

    std::vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> items[i].weight >> items[i].cost;
    }

    std::cout << get_optimal_cost(n, max_weight , items) << '\n';

    return 0;
}

// #include "bigint.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// number types  ------

using BigInteger = long long;
using Rational = long double;

// number types  ------

struct DpState {
    BigInteger min_weight;
    BigInteger cost;
};

struct Item {
    BigInteger weight;
    BigInteger cost;
    size_t reduced_cost; // used as index in dp
};

void update_state(DpState& dest_state, const DpState& source_state, const Item& item) {
    if (dest_state.min_weight == -1 || dest_state.min_weight > source_state.min_weight + item.weight) {
        dest_state.min_weight = source_state.min_weight + item.weight;
        dest_state.cost = source_state.cost + item.cost;
    }
}

void resize_for_update(std::vector<DpState>& dp, const BigInteger& max_weight, const Item& item) {
    size_t max_reduced_cost = dp.size();
    for (size_t i = 0; i < dp.size(); ++i)
        if (dp[i].min_weight != -1 && dp[i].min_weight + item.weight <= max_weight)
            max_reduced_cost = std::max(max_reduced_cost, i + item.reduced_cost + 1);
    dp.resize(max_reduced_cost, DpState{-1, 0});
}

void update_states(std::vector<DpState>& dp, const BigInteger& max_weight, const Item& item) {
    long long old_size = dp.size();
    resize_for_update(dp, max_weight, item);
    for (long long i = old_size - 1; i >= 0; --i) {
        if (dp[i].min_weight != -1 && dp[i].min_weight + item.weight <= max_weight) {
            update_state(dp[i + item.reduced_cost], dp[i], item);
        }
    }
}

// O(n^3/eps) time & O(n^2/eps) memory
BigInteger solve_reduced_problem(
            const BigInteger& max_weight,
            const std::vector<Item>& items) {

    std::vector<DpState> dp_least_weight = {DpState{0, 0}};

    for (const auto& item : items) {
        update_states(dp_least_weight, max_weight, item);
    }

    BigInteger best_cost = 0;
    for (const auto& state : dp_least_weight) {
        if (state.min_weight != -1)
            best_cost = std::max(best_cost, state.cost);
    }
    return best_cost;
}

void reduce_cost(
            std::vector<Item>& items,
            const Rational& precision) {

    // reduced_cost = floor(cost * n / (precision * max_cost))
    size_t n = items.size();
    BigInteger max_cost = 0;
    for (const auto& item : items) {
        max_cost = std::max(max_cost, item.cost);
    }

    Rational reduction_coef;
    if (precision == 0 || max_cost == 0 || n / (precision * max_cost) >= 1)  // Reduction won't help
        reduction_coef = 1;
    else
        reduction_coef = n / (precision * max_cost);

    for (auto& item : items) {
        item.reduced_cost = std::floor(item.cost * reduction_coef);
    }
}

BigInteger get_approximate_optimal_cost(
            const BigInteger& max_weight,
            const Rational& precision,
            const std::vector<BigInteger>& weights,
            const std::vector<BigInteger>& costs) {

    // copy data
    size_t n = weights.size();
    std::vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].weight = weights[i];
        items[i].cost = costs[i];
    }

    // calc reduced cost
    reduce_cost(items, precision);

    return solve_reduced_problem(max_weight, items);
}

int main() {
    int n;
    BigInteger max_weight;
    std::cin >> n >> max_weight;

    std::vector<BigInteger> weights(n);
    std::vector<BigInteger> costs(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> weights[i] >> costs[i];
    }

    Rational precision;
    std::cin >> precision;

    std::cout << get_approximate_optimal_cost(max_weight, precision, weights, costs) << '\n';

    return 0;
}

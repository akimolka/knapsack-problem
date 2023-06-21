#include "bigint.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

// number types  ------

using BigInteger = long long;
using Rational = long double;

bool IsRatioSmallerThenOnePlusEpsilon(BigInteger numerator, BigInteger denominator, const Rational& epsilon) {
    // return numerator / denominator < (1 + epsilon);
    return numerator < denominator * (1 + epsilon);
}

// number types  ------


struct Item {
    BigInteger weight;
    BigInteger cost;
};

using State = Item;

bool operator<(const State& lhs, const State& rhs) {
    // if weight is equal, state with greater cost goes first
    return std::tie(lhs.weight, rhs.cost) < std::tie(rhs.weight, lhs.cost);
}


// gets non-empty ordered set of states, adds new item to each, merges sets
void update(std::vector<State>& states, const Item& new_item) {
    std::vector<State> new_states = states;
    for (int i = 0; i < new_states.size(); ++i) {
        new_states[i].weight += new_item.weight;
        new_states[i].cost += new_item.cost;
    }

    std::vector<State> all_states(2 * states.size());
    std::merge(states.begin(), states.end(),
                new_states.begin(), new_states.end(),
                all_states.begin());

    // remove useless states
    int iter = 0;
    for (int i = 1; i < all_states.size(); ++i) {
        if (all_states[i].cost > all_states[iter].cost) {
            all_states[++iter] = all_states[i];
        }
    }
    all_states.resize(iter + 1);

    std::swap(states, all_states);
}


// removes all states from the end with weight bigger than max_weight
void crop(std::vector<State>& states, BigInteger max_weight) {
    while (states.back().weight > max_weight) {
        states.pop_back();
    }
}

// if there are 3 consecutive states A, B, C, such that cost of C <= (1 + eps) cost of A, B is removed
void sift(std::vector<State>& states, const Rational& precision) {
    if (states.size() <= 2) return;
    int index_a = 0;
    for (int index_c = 2; index_c < states.size(); ++index_c) {
        if (IsRatioSmallerThenOnePlusEpsilon(states[index_c].cost, states[index_a].cost, precision)) {
            states[index_a + 1] = states[index_c];
        } else {
            states[index_a + 2] = states[index_c];
            index_a += 1;
        }
    }
    states.resize(index_a + 2);
}

BigInteger get_approximate_optimal_cost(
            int n,
            BigInteger max_weight,
            const Rational& precision,
            const std::vector<Item>& items) {
    std::vector<State> reachable_states;
    reachable_states.push_back(Item{BigInteger(0), BigInteger(0)});

    for (int i = 0; i < n; ++i) {
        update(reachable_states, items[i]);
        crop(reachable_states, max_weight);
        sift(reachable_states, precision);
    }
    return reachable_states.back().cost;
}

int main() {
    int n;
    BigInteger max_weight;
    std::cin >> n >> max_weight;

    Rational precision;
    std::cin >> precision;

    std::vector<Item> items(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> items[i].weight >> items[i].cost;
    }

    std::cout << get_approximate_optimal_cost(n, max_weight, precision, items) << '\n';

    return 0;
}

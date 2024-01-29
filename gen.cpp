#include <vector>
#include <iostream>
#include <map>

using std::string;
using std::cout;

int rand_from_to(int lower, int upper) { // [ ]
    int shift = rand() % (upper - lower + 1);
    return lower + shift;
}

int main(int argc, char* argv[]) {
    std::map<string, string> params;
    for (int i = 1; i < argc; i++) {
        string s = string(argv[i]);
        if (s.find('=') != string::npos) {
            auto pos = s.find('=');
            params[s.substr(0, pos)] = s.substr(pos + 1);
        }
    }

    std::srand(atoi(argv[1]));

    int n = stoi(params["n"]);
    int max_weight = stoi(params["max_weight"]);
    long long cost = stoll(params["cost"]);
    long double precision = 0.0;
    if (params.count("precision"))
        precision = std::stold(params["precision"]);

    cout << n << " " << max_weight << "\n";
    int approximate_weight = std::max(max_weight / n, 1);
    for (int i = 0; i < n; i++) {
        cout << rand() % (approximate_weight * 5) << " ";
        cout << rand() % cost + 1 << "\n";
    }
    cout << precision;
}
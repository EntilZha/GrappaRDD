#include <iostream>
#include <vector>
#include <algorithm>
#include "GrappaContext.hpp"

using namespace std;

int main() {
    cout << "Hello, World!" << endl;
    vector<int> data {1, 2, 3, 4};
    auto context = new GrappaContext;
    auto rdd = context->parallelize(data);
    auto r = rdd->map([](int a) -> double {
        return a * 2;
    })->map([](double a) -> double {
        return a * 2;
    });

    cout << "Printing numbers\n";
    print_vector(r->compute());

    cout << "Printing sum\n";
    cout << r->fold(0, [](int a, int b) -> int {
        return a + b;
    });
    return 0;
}

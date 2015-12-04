#include <iostream>
#include <vector>
#include <algorithm>
#include "RDD.hpp"

using namespace std;


template<typename T>
void print_vector(vector<T> v) {
    for (auto i = v.begin(); i != v.end(); ++i) {
        cout << *i << ' ' << endl;
    }
}


int main() {
    cout << "Hello, World!" << endl;
    vector<int> data {1, 2, 3, 4};
    MappedRDD<int> rdd(data);
    auto r = rdd.map([](int a) -> double {
        return a * 2;
    }).map([](double a) -> double {
        return a + 5;
    });

    cout << "Printing numbers\n";
    print_vector(r.sequence);

    cout << "Printing sum\n";
    cout << r.fold(0, [](int a, int b) -> int {
        return a + b;
    });
    return 0;
}
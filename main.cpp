#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int main() {
    cout << "Hello, World!" << endl;
    vector<int> data {1, 2, 3, 4};
//    MappedRDD<int> rdd(data);
//    auto r = rdd.map([](int a) -> double {
//        return a * 2;
//    }).map([](double a) -> double {
//        return a + 5;
//    });
//    for (auto i = r.sequence.begin(); i != r.sequence.end(); ++i) {
//        cout << *i << ' ' << endl;
//    }
//    cout << r.fold(0, [](int a, int b) -> int {
//        return a + b;
//    });
    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename A>
class RDD {
public:
    vector<A> sequence;
    RDD(vector<A> sequence): sequence(sequence) {}

    template<typename Func>
    auto map(Func f) -> RDD<decltype(f(A()))> {
        vector<decltype(f(A()))> result;
        result.resize(sequence.size());
        std::transform(sequence.begin(), sequence.end(), result.begin(), f);
        return RDD<decltype(f(A()))>(result);
    }

    template<typename Func>
    auto fold(A init, Func f) -> decltype(f(A(), A())) {
        return std::accumulate(std::begin(sequence), std::end(sequence), init, f);
    }
};

int main() {
    cout << "Hello, World!" << endl;
    vector<int> data {1, 2, 3, 4};
    RDD<int> rdd(data);
    auto r = rdd.map([](int a) -> double {
        return a * 2;
    });
    for (auto i = r.sequence.begin(); i != r.sequence.end(); ++i) {
        cout << *i << ' ' << endl;
    }
    cout << r.fold(0, [](int a, int b) -> int {
        return a + b;
    });
    return 0;
}
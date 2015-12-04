#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<typename A>
class MappedRDD;

template <typename A>
class RDD {
public:
    vector<A> sequence;

    RDD() {}
    RDD(vector<A> sequence): sequence(sequence) {}

    template<typename Func>
    auto map(Func f) -> RDD<decltype(f(A()))> {
        vector<decltype(f(A()))> result;
        result.resize(sequence.size());
        std::transform(sequence.begin(), sequence.end(), result.begin(), f);
        return MappedRDD<decltype(f(A()))>(result);
    }

    template<typename Func>
    auto fold(A init, Func f) -> decltype(f(A(), A())) {
        return std::accumulate(std::begin(sequence), std::end(sequence), init, f);
    }

//    virtual vector<A> compute() = 0;
};

template <typename A>
class MappedRDD: public RDD<A> {
public:
    MappedRDD(vector<A> sequence): RDD<A>(sequence) {}
//    vector<A> compute() {
//        vector<A> result;
//        return result;
//    }
};

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
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
//        return this->sequence;
//    }
};


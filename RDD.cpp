#include "RDD.h"
#include <vector>
#include <algorithm>

using namespace std;

template <typename A, typename Func>
auto RDD<A>::map(Func f) -> RDD<decltype(f(A()))> {
    vector<decltype(f(A()))> result;
    result.resize(sequence.size());
    std::transform(sequence.begin(), sequence.end(), result.begin(), f);
    return MappedRDD<decltype(f(A()))>(result);
}

template<typename A, typename Func>
auto RDD<A>::fold(A init, Func f) -> decltype(f(A(), A())) {
    return std::accumulate(std::begin(sequence), std::end(sequence), init, f);
}

template <typename A>
MappedRDD<A>::MappedRDD(vector<A> sequence): sequence(sequence) {}

template <typename A>
vector<A> MappedRDD<A>::compute() {
    vector<A> result;
    return result;
}


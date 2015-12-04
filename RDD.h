//
// Created by Pedro Rodriguez on 12/2/15.
//
#include <functional>

using std::function;

#ifndef GRAPPARDD_RDD_H
#define GRAPPARDD_RDD_H

#include <vector>
#include <algorithm>

//using namespace std;
//
//template <typename A>
//class RDD {
//public:
//    vector<A> sequence;
//
//    template<typename Func>
//    auto map(Func f) -> RDD<decltype(f(A()))>;
//
//    template<typename Func>
//    auto fold(A init, Func f) -> decltype(f(A(), A()));
//
//    virtual vector<A> compute() = 0;
//};
//
//template <typename A>
//class MappedRDD: public RDD<A> {
//public:
//    MappedRDD(vector<A> sequence);
//
//    vector<A> sequence;
//
//    vector<A> compute();
//};

#endif //GRAPPARDD_RDD_H

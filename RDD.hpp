#include <vector>
#include <algorithm>
#include <iostream>
#include <Grappa.hpp>

#ifndef GRAPPARDD_RDD_H
#define GRAPPARDD_RDD_H

using namespace std;
using namespace Grappa;

template<typename T>
void print_vector(vector<T> v) {
    cout << "Length: " << v.size() << endl;
    for (auto i = v.begin(); i != v.end(); ++i) {
        cout << *i << ' ' << endl;
    }
}


template<typename A, typename B, typename Func>
class MappedRDD;

template <typename A>
class RDD {
public:

	//template<typename Func>
	//auto map(Func f) -> RDD<decltype(f(A()))>* {
		//return new MappedRDD<A, decltype(f(A())), Func>(this, f);
	//}

	//template<typename Func>
	//auto fold(A init, Func f) -> decltype(f(A(), A())) {
		//auto sequence = compute();
		//return std::accumulate(std::begin(sequence), std::end(sequence), init, f);
	//}

	//auto collect() -> vector<A> {
		//return compute();
	//}

	void print() {
		auto rdd = compute();
		forall(rdd, 10, [](A& e) {
			cout << e << endl;
		});
	}

protected:
    virtual GlobalAddress<A> compute() = 0;
};

//template <typename A, typename B, typename Func>
//class MappedRDD: public RDD<B> {
//public:
	//RDD<A> *prev;
	//Func f;
	//MappedRDD(RDD<A> *prev, Func f): prev(prev), f(f) {}
	//vector<B> compute() {
		//vector<A> prev_vector = prev->compute();
		//vector<decltype(f(A()))> result;
		//result.resize(prev_vector.size());
		//std::transform(prev_vector.begin(), prev_vector.end(), result.begin(), f);
		//return result;
	//}
//};

//template <typename A>
//class ParallelCollectionRDD: public RDD<A> {
	//vector<A> sequence;

//public:
	//ParallelCollectionRDD(vector<A> sequence): sequence(sequence) {}

//protected:
	//GlobalAddress<A> compute() {
		//return sequence;
	//}
//};

class RangedRDD: public RDD<double> {
	int start;
	int end;
public:
	RangedRDD(int start, int end): start(start), end(end) {}

	GlobalAddress<double> compute() {
		auto sequence = global_alloc<double>(end - start);
		forall(sequence, end - start, [](int64_t i, double& e) {
			e = i;
		});

		return sequence;
	}
};

#endif //GRAPPARDD_RDD_H


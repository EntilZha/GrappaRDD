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

	template<typename Func>
	auto map(Func f) -> RDD<decltype(f(A()))>* {
		return new MappedRDD<A, decltype(f(A())), Func>(this, f);
	}

	//template<typename Func>
	//auto fold(A init, Func f) -> decltype(f(A(), A())) {
		//auto sequence = compute();
		//return std::accumulate(std::begin(sequence), std::end(sequence), init, f);
	//}

	auto collect() -> vector<A> {
		auto rdd = compute();
		vector<A> output;
		forall(rdd, size, [&output](A& e) {
			delegate::call<async>(0, [&output, e] {
				output.push_back(e);
			});
		});
		return output;
	}

	void print() {
		auto rdd = compute();
		forall(rdd, size, [](A& e) {
			cout << "Core: "<< rdd.core() << " ";
			cout << e << endl;
		});
	}

    virtual GlobalAddress<A> compute() = 0;
	int size;
};

template <typename A, typename B, typename Func>
class MappedRDD: public RDD<B> {
public:
	RDD<A> *prev;
	Func f;
	MappedRDD(RDD<A> *prev, Func f): prev(prev), f(f) {}

	GlobalAddress<B> compute() {
		// Assumes sizeof(A)=sizeof(B)
		GlobalAddress<A> prev_rdd = prev->compute();
		typedef GlobalAddress<decltype(f(A()))> result_type;
		result_type rdd = static_cast<result_type>(prev_rdd);
		forall(rdd, this->size, [this, prev_rdd](int64_t i, A& e) {
			e = this->f(*(prev_rdd + i).pointer());
		});
		return rdd;
	}
};

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
	RangedRDD(int start, int end): start(start), end(end) {
		size = end - start;
	}

	GlobalAddress<double> compute() {
		auto sequence = global_alloc<double>(end - start);
		forall(sequence, end - start, [](int64_t i, double& e) {
			e = i;
		});

		return sequence;
	}
};

#endif //GRAPPARDD_RDD_H


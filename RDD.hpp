#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
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


template<typename A, typename B>
class MappedRDD;

template <typename A>
class RDD {
public:

	template<typename Func>
	auto map(Func f) -> RDD<decltype(f(A()))>* {
		return new MappedRDD<A, decltype(f(A()))>(this, f);
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
			cout << e << endl;
		});
	}

    virtual GlobalAddress<A> compute() = 0;
	int size;

protected:
	GlobalAddress<A> rdd_address;
};

template <typename A, typename B>
class MappedRDD: public RDD<B> {
public:
	RDD<A> *prev;
	std::function<B(A)> f;
	MappedRDD(RDD<A> *prev, std::function<B(A)> f): prev(prev), f(f) {
		this->size = prev->size;
	}

	GlobalAddress<B> compute() {
		// Assumes sizeof(A)=sizeof(B)
		auto prev_rdd = prev->compute();
		this->rdd_address = static_cast<GlobalAddress<B>>(prev_rdd);

		forall(this->rdd_address, this->size, [this](int64_t i, A& e) {
			e = this->f(e);
		});
		return this->rdd_address;
	}
};

template <typename A>
class ParallelCollectionRDD: public RDD<A> {
	vector<A> sequence;
public:
	ParallelCollectionRDD(vector<A> sequence): sequence(sequence) {
		this->size = sequence.size();
	}

protected:
	GlobalAddress<A> compute() {
		this->rdd_address = global_alloc<A>(this->size);
		forall(this->rdd_address, this->size, [this](int64_t i, A& e) {
			e = this->sequence[i];
		});
		return this->rdd_address;
	}
};

class RangedRDD: public RDD<double> {
	int start;
	int end;
public:
	RangedRDD(int start, int end): start(start), end(end) {
		size = end - start;
	}

	GlobalAddress<double> compute() {
		this->rdd_address = global_alloc<double>(end - start);
		forall(this->rdd_address, this->size, [](int64_t i, double& e) {
			e = i;
		});

		return this->rdd_address;
	}
};

#endif //GRAPPARDD_RDD_H


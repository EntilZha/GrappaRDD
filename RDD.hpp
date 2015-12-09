#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <type_traits>
#include <typeinfo>
#include <Grappa.hpp>


#ifndef GRAPPARDD_RDD_H
#define GRAPPARDD_RDD_H

using namespace std;
using namespace Grappa;

template<typename T>
void print_vector(vector <T> v) {
    cout << "Length: " << v.size() << endl;
    for (auto i = v.begin(); i != v.end(); ++i) {
        cout << *i << ' ' << endl;
    }
}


template<typename T, typename A>
class MappedRDD;

template<typename A>
class RDD {
public:

    template<typename Func>
    auto map(Func f) -> RDD<decltype(f(A()))> * {
        return new MappedRDD<A, decltype(f(A()))>(this, f);
    }

    typedef A(*fold_f_t)(const A&, const A&);
    auto fold(A init, fold_f_t f) -> A {
        auto rdd = this->compute();

        A value = init;
        A *value_addr = &value;
        forall(rdd, size, [f, value_addr](int64_t start, int64_t n, A *ptr) {
            for (auto i = 0; i < n; i++) {
                *value_addr = f(*value_addr, *ptr + i);
            }
        });

        A final_value = allreduce<A, f>(value);
        return A();
    }

    auto collect() -> vector <A> {
        auto rdd = this->compute();
        vector <A> output;
        forall(rdd, size, [&output](A &e) {
            delegate::call<async>(0, [&output, e] {
                output.push_back(e);
            });
        });
        return output;
    }

    void print() {
        auto rdd = this->compute();
        forall(rdd, size, [](A &e) {
            cout << e << endl;
        });
    }

    virtual GlobalAddress <A> compute() = 0;

    int size;

protected:
    GlobalAddress <A> rdd_address;
};

template<typename T, typename A>
class MappedRDD : public RDD<A> {
public:
    static_assert(sizeof(A) == sizeof(T), "A and T must have the same size");
    RDD<T> *prev;
    std::function<A(T)> f;

    MappedRDD(RDD<T> *prev, std::function<A(T)> f) : prev(prev), f(f) {
        this->size = prev->size;
    }

    GlobalAddress <A> compute() {
        // Assumes sizeof(A)=sizeof(T)
        auto prev_address = prev->compute();
        this->rdd_address = static_cast<GlobalAddress <A>>(prev_address);
        auto func = this->f;

        forall(this->rdd_address, this->size, [func](int64_t i, A &e) {
            e = func((T &) e);
        });
        return this->rdd_address;
    }
};

//template<typename A>
//ParallelCollectionRDD<A> parallelize(vector<A> sequence) {
//return new ParallelCollectionRDD(sequence);
//}

template<typename A>
class ParallelCollectionRDD : public RDD<A> {
private:
    vector <A> sequence;
public:
    ParallelCollectionRDD(vector <A> sequence) : sequence(sequence) {
        this->size = sequence.size();
    }

    GlobalAddress <A> compute() {
        this->rdd_address = global_alloc<A>(this->size);
        for (int i = 0; i < this->size; i++) {
            delegate::write(this->rdd_address + i, this->sequence[i]);
        }
        return this->rdd_address;
    }
};

template<typename A>
class RangedRDD : public RDD<A> {
    A start;
    A end;
public:
    RangedRDD(A start, A end) : start(start), end(end) {
        this->size = static_cast<int>(end - start);
    }

    GlobalAddress <A> compute() {
        this->rdd_address = global_alloc<A>(this->size);
        forall(this->rdd_address, this->size, [](int64_t i, A &e) {
            e = i;
        });

        return this->rdd_address;
    }
};

#endif //GRAPPARDD_RDD_H


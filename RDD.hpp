/**
    @author Pedro Rodriguez 
*/

#include <vector>
#include <iostream>
#include <Grappa.hpp>


#ifndef GRAPPARDD_RDD_H
#define GRAPPARDD_RDD_H

using std::cout;
using std::vector;
using std::endl;
using namespace Grappa;

namespace GrappaRDD {
    template<typename T>
    void print_vector(vector <T> v) {
        cout << "Length: " << v.size() << endl;
        for (auto i = v.begin(); i != v.end(); ++i) {
            cout << *i << ' ' << endl;
        }
    }

    template<typename A>
    A add(const A& left, const A& right) {
        return left + right;
    }

    template<typename A>
    A product(const A& left, const A& right) {
        return left * right;
    }
}


template<typename T, typename A>
class MappedRDD;

template<typename A>
class RangedRDD;

template<typename A>
class RDD {
public:
    template<typename T, typename U>
    friend class MappedRDD;

    static RangedRDD<A>* range(A start, A end);
    static RangedRDD<A>* range(A end);


    template<typename Func>
    auto map(Func f) -> RDD<decltype(f(A()))> * {
        return new MappedRDD<A, decltype(f(A()))>(this, f);
    }

    /**
        Folds the RDD to a single value using f.

        @param init Initial vale for fold
        @param f Function which folds elements by (A, A) -> A
        @return RDD<A> reduced using f
    */
    auto fold(A init, A (*f)(const A&, const A&)) -> A {
        auto rdd = this->compute();

        A global_value = init;

        auto rdd_address = this->rdd_address;
        auto size = this->size;
        auto global_value_addr = make_global(&global_value);

        finish([f, rdd_address, size, global_value_addr] {
            on_all_cores([rdd_address, size, global_value_addr, f]{
                A *local_start = rdd_address.localize();
                A *local_end = (rdd_address + size).localize();
                A local_value = *local_start;
                if (local_end > local_start) {
                    for(A *val = local_start + 1; val < local_end; ++val) {
                        local_value = f(local_value, *val);
                    }
                }
                delegate::call<async>(global_value_addr, [local_value, f] (A &global_value) {
                    global_value = f(global_value, local_value);
                });
            });
        });

        return global_value;
    }

    A sum() {
        return this->fold(A(), GrappaRDD::add);
    }

    A product() {
        return this->fold(A(), GrappaRDD::product);
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
    virtual void free() {
        cout << "Parent free called" << endl;
    };

    ~RDD<A>() {
        cout << "RDD destructor called" << endl;
        this->free();
    }
protected:
    int size;
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

    virtual void free() override {
        prev->free();
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
        auto size = this->size;
        auto rdd_address = this->rdd_address;
        auto sequence = this->sequence;
        finish([size, rdd_address, sequence]{
            for (int i = 0; i < size; i++) {
                delegate::write<async>(rdd_address + i, sequence[i]);
            }
        });
        return this->rdd_address;
    }

    virtual void free() override {
        global_free(this->rdd_address);
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

    virtual void free() override {
        global_free(this->rdd_address);
        cout << "RDD freed" << endl;
    }
};

template<typename A>
RangedRDD<A>* RDD<A>::range(A start, A end) {
    return new RangedRDD<A>(start, end);
}

template<typename A>
RangedRDD<A>* RDD<A>::range(A end) {
    return new RangedRDD<A>(0, end);
}

#endif //GRAPPARDD_RDD_H


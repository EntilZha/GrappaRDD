#include "RDD.hpp"

#ifndef GRAPPARDD_GRAPPACONTEXT_H
#define GRAPPARDD_GRAPPACONTEXT_H


class GrappaContext {
public:
    template <typename A>
    static ParallelCollectionRDD<A>* parallelize(vector<A> sequence) {
        return new ParallelCollectionRDD<A>(sequence);
    }
};

#endif //GRAPPARDD_GRAPPACONTEXT_H

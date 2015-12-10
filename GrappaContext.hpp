#include "RDD.hpp"
#include "Grappa.hpp"

#ifndef GRAPPARDD_GRAPPACONTEXT_H
#define GRAPPARDD_GRAPPACONTEXT_H


class GrappaContext {
private:
    bool closed = true;
public:
    GrappaContext(int argc, char *argv[]) {
        Grappa::init(&argc, &argv);
        closed = false;
    }

    ~GrappaContext() {
        if (!closed) {
            Grappa::finalize();
            closed = true;
        }
    }

    void run(void(*fp)()) {
        Grappa::run(fp);
    }

    void stop() {
        this->~GrappaContext();
        closed = true;
    }
};

#endif //GRAPPARDD_GRAPPACONTEXT_H

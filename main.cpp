#include <iostream>
#include <vector>
#include <stdio.h>
#include <typeinfo>
#include "GrappaContext.hpp"

using namespace std;
using namespace Grappa;

struct int_container {
    int64_t a;
    int64_t b;
};

struct double_container {
    double a;
    double b;
};


int main(int argc, char *argv[]) {
    init(&argc, &argv);


    run([] {
        //cout << "Simple print" << endl;
        //(new RangedRDD<int64_t>(0, 10))->print();

        cout << "Mapped print" << endl;
        auto f = [](int64_t a) -> double {
            return a * 2.5;
        };

        auto rdd = new RangedRDD<int64_t>(0, 10);
        rdd->map(f)->print();

        on_all_cores([] {
            cout << "Core Count: " << mycore() << endl;
        });

        vector<int> coll_data {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        auto collection_rdd = new ParallelCollectionRDD<int>(coll_data);
        print_vector(collection_rdd->collect());

        //cout << "ParallelCollectionRDD\n";
        //vector <int_container> data;
        //for (int i = 0; i < 20; i++) {
            //int_container v;
            //v.a = 2 * i;
            //v.b = i;
            //data.push_back(v);
        //}

        //auto pcoll = new ParallelCollectionRDD<int_container>(data);
        //auto mpcoll = pcoll->map([](int_container a) -> double_container {
            //double_container v;
            //v.a = 2.5;
            //v.b = 1.5;
            //return v;
        //});
        //for (auto e: mpcoll->collect()) {
            //cout << "a: " << e.a << " b: " << e.b << endl;
        //}

    });
    //cout << "Printing numbers\n";
    //print_vector(r->compute());

    //cout << "Printing sum\n";
    //cout << r->fold(0, [](int a, int b) -> int {
    //return a + b;
    //});

    //auto s = r->map([](double a) -> string {
    //string prefix("number");
    //string result = prefix + std::to_string(a);
    //return result;
    //});
    //print_vector(s->collect());

    finalize();

    return 0;
}


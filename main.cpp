#include <iostream>
#include <vector>
#include "GrappaContext.hpp"
#include <time.h>
#include <iomanip>

using std::cout;

struct int_container {
    int64_t a;
    int64_t b;
};

struct double_container {
    double a;
    double b;
};

int add(const int& a, const int& b) {
    return a + b;
}


int main(int argc, char *argv[]) {
    auto gc = new GrappaContext(argc, argv);
    gc->run([] {
        struct timeval t0, t1;
        for (int i = 0; i < 10; i++) {
            gettimeofday(&t0, NULL);
            auto rdd = RDD<int64_t>::range(1000000000)->map([](const int64_t& a){return a;});
            auto sum = rdd->sum();
            gettimeofday(&t1, NULL);
            auto ms = (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) / 1000;
            cout << ms << endl;
            delete rdd;
        }
        //vector<int> coll_data(100);
        //for (int i = 0; i < 100; i++) {
            //coll_data[i] = i;
        //}
        //auto collection_rdd = new ParallelCollectionRDD<int>(coll_data);
        //int sum = collection_rdd->fold(0, [](const int& a, const int& b) -> int {return a + b;});
        //cout << "Sum: " << sum << endl;

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
    gc->stop();

    return 0;
}


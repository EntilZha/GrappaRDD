#include <iostream>
#include <vector>
#include "GrappaContext.hpp"

using namespace std;
using namespace Grappa;

struct container {
	double a;
	int b;
};

int main(int argc, char * argv[]) {
	init(&argc, &argv);


	run([] {
		//cout << "Simple print" << endl;
		//(new RangedRDD<int64_t>(0, 10))->print();

		cout << "Mapped print" << endl;
		(new RangedRDD<int64_t>(0, 10))->map([](int64_t a) -> double {
			return a * 2.5;
		})->print();

		on_all_cores([]{
			cout << "Core Count: " << mycore() << endl;
		});

		cout << "ParallelCollectionRDD\n";
    	vector<container> data;
		for (int i = 0; i < 20; i++) {
			container v;
			v.a = 1.5 * i;
			v.b = i;
			data.push_back(v);
		}

		auto pcoll = new ParallelCollectionRDD<container>(data);
		for (auto e: pcoll->collect()) {
			cout << "a: " << e.a << " b: " << e.b << endl;
		}

		vector<int64_t> data2 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		auto pcoll2 = new ParallelCollectionRDD<int64_t>(data2);

		cout << "Mapped ParallelCollectionRDD\n";
		auto mpoll2 = pcoll2->map([](int64_t a) {
			return a * 2;
		});

		for (auto e: mpoll2->collect()) {
			cout << "ce: " << e << endl;
		}
	});

    //auto rdd = context->parallelize(data);
    //auto r = rdd->map([](int a) -> double {
        //return a * 2;
    //})->map([](double a) -> double {
        //return a * 2;
    //});

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


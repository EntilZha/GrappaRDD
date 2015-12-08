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
		auto rdd = new RangedRDD(0, 10);
		cout << "Simple print" << endl;
		//rdd->print();
		//print_vector(rdd->collect());
		cout << "Mapped print" << endl;
		rdd->map([](double a) {
			return a * 2;
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
		auto result = pcoll->collect();
		for (int i = 0; i < 20; i ++) {
			cout << "a: " << result[i].a << " b: " << result[i].b << endl;
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


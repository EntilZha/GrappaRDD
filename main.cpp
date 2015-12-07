#include <iostream>
#include <vector>
#include "GrappaContext.hpp"

using namespace std;
using namespace Grappa;

int main(int argc, char * argv[]) {
		init(&argc, &argv);

		run([]{
		auto f = [](int64_t a) {
			return a + 5;
		};

		auto array = global_alloc<int64_t>(10);
			forall(array, 9, [f](int64_t i, int64_t& n) {
					n = i;
			});

			forall(array, 10, [f](int64_t& n) {
				cout << f(n) << endl;
			});
		});

		cout << "\n";



		//forall(array, 10, [](int64_t& e) {
		//		cout << e << endl;
		//});

		finalize();


    cout << "Hello, World!" << endl;
    vector<int> data {1, 2, 3, 4};
    auto context = new GrappaContext;
    auto rdd = context->parallelize(data);
    auto r = rdd->map([](int a) -> double {
        return a * 2;
    })->map([](double a) -> double {
        return a * 2;
    });

    cout << "Printing numbers\n";
    print_vector(r->compute());

    cout << "Printing sum\n";
    cout << r->fold(0, [](int a, int b) -> int {
        return a + b;
    });

    auto s = r->map([](double a) -> string {
        string prefix("number");
        string result = prefix + std::to_string(a);
        return result;
    });
    print_vector(s->collect());
    return 0;
}


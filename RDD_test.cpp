#include "Grappa.hpp"
#include "RDD.hpp"
#define BOOST_TEST_MODULE RDD_test
#include <boost/test/unit_test.hpp>

template<typename A>
A sum(A start, A end) {
    A summed = 0;
    for (auto i = 0; i < end; i++) {
        summed += i;
    }
    return summed;
}

BOOST_AUTO_TEST_CASE(rdd_range_test) {
    init(GRAPPA_TEST_ARGS);

    run([] {
        int64_t n = 100;
        auto rdd = new RangedRDD<int64_t>(0, n);
        auto rdd_sum = rdd->fold(0, [](const int64_t& a, const int64_t& b) {
            return a + b;
        });

        int64_t sum_expect = sum(0, 100);

        BOOST_CHECK_EQUAL(rdd_sum, sum_expect);

        auto double_sum = rdd->map([](const int64_t a) -> double {
            return a * 1.5;
        })->fold(0, [](const double& a, const double& b) {
            return a + b;
        });

        BOOST_CHECK_EQUAL(double_sum, sum_expect * 1.5);

        auto numbers = rdd->collect();
        BOOST_CHECK_EQUAL(numbers.size(), n);
        for (int i = 0; i < n; i++) {
            BOOST_CHECK(std::find(numbers.begin(), numbers.end(), i) != numbers.end());
        }

    });

    finalize();
}


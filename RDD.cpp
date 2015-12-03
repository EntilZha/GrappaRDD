template <typename A>
class RDD {
public:
    template<typename Func>
    auto map(Func f) -> RDD<decltype(f())> {
        RDD<decltype(f(A()))> rdd;
        return rdd;
    }
};


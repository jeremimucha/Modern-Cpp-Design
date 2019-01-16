#include <iostream>
#include <type_traits>
#include <utility>
#include <memory>

/**
 * Mapping integral constants to types.
 * The fact that template instantitations are distinct types (including instantiations of non-type
 * templates) can be used to utilise template specialization or partial specialization or function
 * overloading. Thus we can use conditions evaluated at compile-time to achieve static dispatching
 * on an integral value.
 */

template<typename T, T Value>
struct ct_value {
    using type = ct_value;
    using value_type = T;
    static constexpr inline T value{Value};
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

template<bool value>
using ct_bool = ct_value<bool, value>;

template<typename T, bool is_polymorphic>
class NiftyContainer {
private:
    // ...
    void do_something(T* p, ct_bool<true>)
    {
        std::unique_ptr<T> pNewObj = p->clone();
        // polymorphic algorithm
    }

    void do_something(T* p, ct_bool<false>)
    {
        T newObj{*p};
        // non-polymorphic algorithm
    }

public:
    void do_something(T* p)
    {
        do_something(p, ct_bool<is_polymorphic>{});
    }
};

// or maybe:
template<typename T, bool is_polymorphic>
class NiftyContainer;

template<typename T>
class NiftyContainer<T, true> {     // polymorphic implementation
public:
    void do_something(T* p)
    {
        std::unique_ptr<T> pNewObj = p->clone();
        // polymorphic algorithm
    }
};

template<typename T>
class NiftyContainer<T, false> {    // non-polymorphic implementation
public:
    void do_something(T* p)
    {
        T newObj{*p};
        // non-polymorphic algorithm
    }
};



int main()
{

}

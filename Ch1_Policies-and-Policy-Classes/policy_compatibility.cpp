#include <iostream>


/**
 * When implementing multiple policies it's worth considering if client code should be able to
 * convert from a template using one policy implementation to another. It's a matter of correctly
 * defining the constructors syntactically so that conversions make sens semantically.
 */

// Given:
template< typename T,
          template<class> class CheckingPolicy,
          template<class> class ThreadingModel
        >
class SmartPtrT : public CheckingPolicy<T>, public ThreadingModel<T> {
    // ...
};

struct Widget { };

// and policies
struct NoChecking { };
struct EnforceNotNull { };
struct SingleThreaded { };
struct MultiThreaded { };

// It is worth considering if conversions from
//  using FastWidgetPtr = SmartPtr<Widget, NoChecking, SingleThreaded>;
// to
//  using SafeWidgetPtr = SmartPtr<Widget, EnforceNotNull, SingleThreaded>;
// should be allowed? Conversions from FastWidgetPtr -> SafeWidgetPtr are most likely fine,
// the other way around, not so much.

// The way to achieve this is to construct policy-by-policy (simplified to just one policy)
template< typename T, template<class> class CheckingPolicy>
class SmartPtr : public CheckingPolicy<T> {
public:
    // ...
    template<typename T1, template<class> class CP>
    SmartPtr(SmartPtr<T1,CP> const& other)
        : CheckingPolicy<T>{other}, ptr_{other.ptr_}
    {
    }

    // ...

private:
    T* ptr_;
};

// Given the above the compiler will allow the instantiation only if T1 is convertible to T
// and if the CP is convertible to CheckingPolicy<T>, which would be possible if CheckingPolicy<T>
// defines a non-explicit constructor that takes CP or if CP defines a conversion to CheckingPolicy.
// This could further be explicitly enforced by using enable_if or concepts.

int main()
{

}

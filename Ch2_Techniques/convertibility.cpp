#include <iostream>
#include <type_traits>  // true_type, false_type
#include <utility>      // declval

/**
 * Detecting convertibility and inheritance at compiletime. This can be useful for
 * inheritanc hierarchies, to avoid using dynamic-cast.
 */

template<typename FROM, typename TO>
struct IsConvertibleImpl {
private:
    // test() - trying to call aux() for a value of type FROM
    static void aux(TO);
    template<typename F, typename = decltype(aux(std::declval<F>()))>
    static std::true_type test(void*) noexcept;

    // test() fallback
    template<typename, typename>
    static std::false_type test(...) noexcept;
public:
    using type = decltype(test<FROM>(nullptr));
};

template<typename FROM, typename TO>
struct IsConvertibleT : IsConvertibleImpl<FROM, TO>::type { };

template<typename FROM, typename TO>
using IsConvertible = typename IsConvertibleT<FROM, TO>::type;

template<typename FROM, typename TO>
constexpr inline bool IsConvertible{IsConvertibleT<FROM, TO>::value};

int main()
{

}

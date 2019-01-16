#include <iostream>


/**
 * Considering NiftyContainer again - we might want to store different value types depending on
 * if the type is polymorphic or not. In case of polymorphic types we need to store pointers,
 * in case of non-polymorphic types it's usually more efficient to store by-value.
 * One approach is to use type-traits:
 */
template<typename T, bool is_poly>
struct NiftyContainerTraits {
    using value_type = T*;
};

template<typename T>
struct NiftyContainerTraits<T, false> {
    using value_type = T;
};

template<typename T, bool is_poly>
struct NiftyContainer1 {
    // ...
    using Traits = NiftyContainerTraits<T, is_poly>;
    using value_type = typename Traits::value_type;
    // ...
};

// This works but is somewhat clumsy. We could declare a metafunction that selects a type
// based on a boolean value on the spot, at compile time:
template<bool flag, typename T, typename U>
struct Select {
    using type = T;
};

template<typename T, typename U>
struct Select<false, T, U> {
    using type = U;
};

// Using the Select metafunction:
template<typename T, bool is_poly>
struct NiftyContainer {
    using value_type = Select<is_poly, T*, T>;
    // ...
};

int main()
{

}

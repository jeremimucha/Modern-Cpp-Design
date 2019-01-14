#include <iostream>
#include <string>
#include <memory>
#include <utility>

/**
 * Function partial specialization is not allowed, but sometimes we might want to overload a
 * function based on a type, without passing in the type directly. In such cases we can rely on
 * tag-dispatch to mimic partial specialization using overloading.
 */

template<typename T>
struct Type {
    using type = T;
};

// For example, given:
template<typename T, typename... Args>
std::unique_ptr<T> create(Type<T>, Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

struct Widget { };

template<typename... Args>
std::unique_ptr<Widget> create(Type<Widget>, Args&&... args)
{
    return std::make_unique<Widget>(std::forward<Args>(args)..., -1);
}


int main()
{
    // use create
    auto str = create(Type<std::string>{}, 42, 'C');
    auto widget = create(Type<Widget>{}, "this", "is", 'a', "widget");
}

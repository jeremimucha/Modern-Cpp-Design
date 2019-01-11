#include <iostream>


/**
 * Thanks to C++'s template feature - incomplete instantiation - if a member is not used it will
 * not be instantiated - policies can support optional, enriched interfaces if some extra
 * requirements are met.
 */

// library code
struct Widget { };

template<template<class> class CreationPolicy>
class WidgetManager : public CreationPolicy<Widget> {
    // ...

    // if the CreationPolicy supports `get_prototype()` and `set_prototype()` client code can
    // use the `switch_prototype()` member. The expected interface could be enforced using
    // enable_if, in pseudo-code:
    // std::enable_if_t<Has_get_prototype<CreationPolicy> && Has_set_prototype<CreationPolicy>>
    void switch_prototype(Widget* new_prototype)
    {
    // or via a static_assert here...
    // static_assert(Has_get_prototype<CreationPolicy> && Has_set_prototype<CreationPolicy>);
        delete this->get_prototype();
        this->set_prototype(new_prototype);
    }
};


int main()
{

}

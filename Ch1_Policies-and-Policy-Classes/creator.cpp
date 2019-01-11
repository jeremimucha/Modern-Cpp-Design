#include <iostream>
#include <memory>

/**
 * Policy implementations are called Policy Classes. There can be an unlimited number of
 * implementations of a given policy. Policy Classes are not intended for stand-alone use;
 * instead they are inherited by or contained within other classes.
 */

template<typename T>
struct OpNewCreator {
    static inline T* create()
    {
        return new(std::nothrow) T{};
    }
};

template<typename T>
struct MallocCreator {
    static inline T* create() noexcept
    {
        void* buf = std::malloc(sizeof(T));
        if (!buf) return nullptr;
        return new(buf) T{};
    }
};

template<typename T>
class PrototypeCreator {
public:
    explicit PrototypeCreator(T const* const p) noexcept
        : prototype_{p}
    {
    }

    T* create() const { return prototype_ ? prototype_->clone() : nullptr; }
    T const* get_prototype() const noexcept { return prototype_; }
    void set_prototype(T const* p) noexcept { prototype_ = p; }

private:
    T const* prototype_{nullptr};
};


// library code
template<typename CreationPolicy>
class WidgetManager : public CreationPolicy {
    // ...
};

// as-is users would need to explicitly specify the object that's created by the CreationPolicy,
// It might be the case that the WidgetManager always manages Widgets. We can free the used from
// the burden of specifying the type by making the policy a template template parameter.
// To further ease the lives of our users we can specify a default
struct Widget{};

template<template<typename> class CreationPolicy = OpNewCreator>
class WidgetManager2 : public CreationPolicy<Widget> {
    // ...
};

// WidgetManager might also need to create objects of type `Gadget` using the same creation policy
// given the template template parameter we can:
struct Gadget { };

template<template<typename> class CreationPolicy>
class WidgetManager3 : public CreationPolicy<Widget> {
    // ...
    using CreatorGadget = CreationPolicy<Gadget>;
    void do_something()
    {
        Gadget* pg = CreatorGadget::create();
    }
};


int main()
{

}

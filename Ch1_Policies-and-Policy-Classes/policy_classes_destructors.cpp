#include <iostream>


/**
 * To avoid client code abusing the fact that policies are inherited publically, they need to be
 * implemented in a way that prohibits client code from destructing objects inheriting policy
 * classes via a pointer to a policy class. This is best done by declaring the destructor
 * protected, so that it can be destructed by the inheriting class but not by client code.
 */

template<typename T>
class OpNewCreator {
public:
    static inline T* create() { return new(std::nothrow) T{}; }

protected:
    ~OpNewCreator() = default;
};

struct Widget { };

template<template<class>class CreationPolicy = OpNewCreator>
class WidgetManager : public OpNewCreator<Widget> {
public:
    // ...
};


int main()
{
    // fails at compile-time
    // OpNewCreator<Widget>* p = new WidgetManager{};
    // delete p;   // inaccessible destructor
}

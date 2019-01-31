#include <iostream>

#include "scatter_hierarchy.hpp"

template<typename T>
struct holder {
    T value;
};

using sh1 = scatter_hierarchy<holder, int, double, char>;

int main()
{
    sh1 instance{};
    field<0>(instance).value = 42;

    auto inst2 = scatter_hierarchy(UnitWrapper<holder>{}, typelist<double, double, char>{});
    field<1>(inst2).value = 3.14;
    field<0>(inst2).value = 2.386;
    field<2>(inst2).value = 'C';
    std::cout << field<0>(inst2).value << ", "
              << field<1>(inst2).value << ", "
              << field<2>(inst2).value << "\n";
    std::cout << "sizeof(sh1) == " << sizeof(sh1) << "\n";
    std::cout << "sizeof(inst2) == " << sizeof(inst2) << "\n";
    std::cout << "sizeof(scatter_hierarchy<holder, char, char, char, int>) == "
        << sizeof(scatter_hierarchy<holder, char, char, char, int>) << "\n";
}

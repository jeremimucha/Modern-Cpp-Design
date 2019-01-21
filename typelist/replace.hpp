#pragma once

#include "typelist.hpp"
#include "ifthenelse.hpp"
#include "identity.hpp"
#include <type_traits>


template<typename List, typename S, typename R> struct replace_impl;

template<template<typename...> class List,
         typename S, typename R,
         typename T, typename... Ts>
struct replace_impl<List<T, Ts...>, S, R> {
    using type = List<if_then_else_t<std::is_same_v<T, S>, R, T>,
                      replace_impl<List<Ts...>, S, R>>;
};

template<template<typename...> class List,
         typename S, typename R,
         typename T>
struct replace_impl<List<T>, S, R> {
    using type = List<if_then_else_t<std::is_same_v<T, S>, R, T>>;
};

template<template<typename...> class List,
         typename S, typename R>
struct replace_impl<List<>, S, R>
{
};

template<typename List, typename S, typename R>
struct replace : replace_impl<List, S, R>
{
};

template<typename List, typename S, typename R>
using replace_t = typename replace<List, S, R>::type;

namespace replace_unit_test
{
    using lst1 = typelist<int>;
    using lst2 = typelist<int, double>;
    using lst3 = typelist<int, double, char, double>;
    static_assert(std::is_same_v<replace_t<lst1, int, char>, typelist<char>>);
    static_assert(std::is_same_v<replace_t<lst1, double, char>, lst1>);
    // static_assert(std::is_same_v<replace_t<lst2, int, double>, typelist<double, double>>);

}

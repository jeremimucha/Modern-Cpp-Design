#pragma once

#include "typelist/typelist.hpp"
#include <type_traits>


struct empty_type { };

template<template<class, class> class Unit, typename T, typename... Ts>
class linear_hierarchy : public Unit<T, linear_hierarchy<Unit, Ts...>> { };

template<template<class, class> class Unit, typename T>
class linear_hierarchy<Unit, T> : public Unit<T, empty_type> { };

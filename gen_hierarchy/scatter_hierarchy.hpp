#pragma once

#include "typelist/typelist.hpp"
#include <type_traits>
#include <utility>


template<std::size_t Idx, typename T>
struct hierarchy_element {
public:
    using value_type = T;
    value_type value_;

    // necessary?
    // constexpr hierarchy_element() noexcept = default;
    // template<typename... Ts,
    //          typename = std::enable_if_t<std::is_constructible_v<T, Ts...>>>
    // constexpr explicit hierarchy_element(Ts&&... ts)
    //     noexcept(std::is_nothrow_constructible_v<T, Ts...>)
    //     : value_{std::forward<Ts>(ts)...}
    // {
    // }

    // hierarchy_element(hierarchy_element const&) = default;
    // hierarchy_element(hierarchy_element&&) = default;
    // hierarchy_element& operator=(hierarchy_element const&) = default;
    // hierarchy_element& operator=(hierarchy_element&&) = default;

protected:
    ~hierarchy_element() noexcept = default;
};

template<std::size_t Idx, typename T>
using hierarchy_element_t = typename hierarchy_element<Idx, T>::value_type;


template<template<typename> class Unit, typename Indices, typename... Ts>
class scatter_hierarchy_impl;

template<template<typename> class Unit, std::size_t... Ids, typename... Ts>
class scatter_hierarchy_impl<Unit, std::index_sequence<Ids...>, Ts...>
    : public hierarchy_element<Ids, Unit<Ts>>... {
public:
    using type_list = typelist<Ts...>;
};

template<template<typename> class T>
struct UnitWrapper { };

template<template<typename> class Unit, typename... Ts>
class scatter_hierarchy
    : public scatter_hierarchy_impl<Unit, std::index_sequence_for<Ts...>, Ts...> {

public:
    using Base = scatter_hierarchy_impl<Unit, std::index_sequence_for<Ts...>, Ts...>;
    using type_list = typename Base::type_list;

    constexpr scatter_hierarchy() noexcept = default;
    template<template<typename> class U>
    constexpr scatter_hierarchy(UnitWrapper<U>, typelist<Ts...>) noexcept
    {
    }
};

template<template<typename> class Unit, typename... Ts>
scatter_hierarchy(UnitWrapper<Unit>, typelist<Ts...>) -> scatter_hierarchy<Unit, Ts...>;


template<std::size_t Idx, typename T>
inline constexpr hierarchy_element_t<Idx, T>& field(hierarchy_element<Idx, T>& e) noexcept
{
    return e.value_;
}

template<std::size_t Idx, typename T>
inline constexpr hierarchy_element_t<Idx, T> const&
field(hierarchy_element<Idx, T> const& e) noexcept
{
    return e.value_;
}

template<std::size_t Idx, typename T>
inline constexpr hierarchy_element_t<Idx, T>&& field(hierarchy_element<Idx, T>&& e) noexcept
{
    return std::move(e.value_);
}

template<std::size_t Idx, typename T>
inline constexpr hierarchy_element_t<Idx, T> const&&
field(hierarchy_element<Idx, T> const&& e) noexcept
{
    return std::move(e.value_);
}

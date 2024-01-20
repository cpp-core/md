// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include <type_traits>

namespace core::md {

template<class T, class U, template<class...> class V>
struct is_instance_of_template : std::false_type
{ static constexpr bool value = false; };

template<template<class...> class T, template<class...> class U, class... Ts>
struct is_instance_of_template<T<Ts...>, U<Ts...>, U>
{
    using type = std::true_type;
    static constexpr bool value = std::is_same_v<T<Ts...>,U<Ts...>>;
};

template<class T, template<class...> class U>
using is_same_template_t = typename is_instance_of_template<T,T,U>::type;

template<class T, template<class...> class U>
static constexpr bool is_instance_of_template_v = is_instance_of_template<T,T,U>::value;

}; // core::md

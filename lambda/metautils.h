#pragma once
#include <type_traits>


template <typename _Type>
struct is_const_ref
{
	static const bool value = std::is_const<typename std::remove_reference<_Type>::type>::value;
};

template<typename _Type>
struct PrintType_s;

template<typename _Type>
using PrintType = typename PrintType_s<_Type>::type;

template<typename Base, typename Derived>
using disable_if_same_or_derived_t =
std::enable_if_t <
!std::is_base_of<Base, std::decay_t<Derived>>::value
>;

template<typename Type>
struct type_define
{
	typedef Type type;
};

template<class...> struct types{ using type = types; };

template<template<class...> class test, class lhs, class rhs>
struct zip_test;


template<
	template<class...> class test, 
	class L0, class... lhs,
	class R0, class... rhs>
struct zip_test<test, types<L0, lhs...>, types<R0, rhs...>> :
	std::conditional <
	test < L0, R0 >::value,
	zip_test<test, types<lhs...>, types<rhs...>>,
	std::false_type > ::type
{};

template<
	template<class...> class test,
class... rhs>
struct zip_test<test, types<>, types<rhs...>> :
	std::false_type
{};

template<
	template<class...> class test,
class... lhs>
struct zip_test<test, types<lhs...>, types<>> :
	std::false_type
{};

template<template<class...> class test>
struct zip_test<test, types<>, types<>>
	: std::true_type
{};

template<bool first, bool... vals>
struct is_all : std::false_type {};

template<bool... vals>
struct is_all<true, vals...> : is_all < vals... > {};

template<>
struct is_all<true> : std::true_type {};


template<>
struct is_all<false> : std::false_type{};

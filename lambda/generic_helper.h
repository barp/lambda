#pragma once
#include <type_traits>
#include <utility>


#define CREATE_OVERLOADED_METHOD_FINDER(method_name) \
	template <typename _Ty, typename... Args> \
	class has_##method_name##_method \
	{ \
		template <typename C, typename = decltype(std::declval<C>().method_name(std::declval<Args>()...))> \
		static std::true_type test(int); \
		template <typename C> \
		static std::false_type test(...); \
	public: \
		static constexpr bool value = decltype(test<_Ty>(0))::value; \
	};

#define GENERATE_GENERIC_FORWARD_METHOD(method_name) \
	template <typename _Ty, typename _Uy> \
	inline decltype(auto) method_name(_Ty&& container, _Uy&& value, std::enable_if_t<has_##method_name##_method<_Ty, _Uy>::value>* = 0) \
	{ \
		return std::forward<_Ty>(container).method_name(std::forward<_Uy>(value)); \
	} \
	template <typename _Ty, typename _Uy> \
	inline decltype(auto) method_name(_Ty&& container, _Uy&& value, std::enable_if_t<!has_##method_name##_method<_Ty, _Uy>::value>* = 0) \
	{ \
		return std::method_name(std::forward<_Ty>(container).begin(), std::forward<_Ty>(container).end(), std::forward<_Uy>(value)); \
	}

namespace lambda {

	CREATE_OVERLOADED_METHOD_FINDER(find);
	GENERATE_GENERIC_FORWARD_METHOD(find);

	CREATE_OVERLOADED_METHOD_FINDER(count);
	GENERATE_GENERIC_FORWARD_METHOD(count);

}
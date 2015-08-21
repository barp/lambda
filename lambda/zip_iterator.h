#pragma once
#include <iterator>
#include <tuple>
#include "metautils.h"

namespace lambda {

	template<typename _Type>
	struct deref_return_type
	{
		typedef decltype(*std::declval<_Type>()) type;
	};

	template<typename... _Iterators>
	struct iterators_tuple_return_type
	{
		typedef std::tuple<typename deref_return_type<_Iterators>::type...> type;
	};

	template<typename... _Iterators>
	using iterators_tuple_return_type_t = typename iterators_tuple_return_type<_Iterators...>::type;

	template<typename CurrentMax, typename... Tags>
	struct maximum_iterator_catagory;

	template<typename CurrentMax, typename Tag0, typename... Tags>
	struct maximum_iterator_catagory<CurrentMax, Tag0, Tags...> :
		std::conditional<std::is_base_of<CurrentMax, std::output_iterator_tag>::value,
		type_define<std::output_iterator_tag>,
		typename std::conditional <
		std::is_base_of<Tag0, std::output_iterator_tag>::value,
		type_define<std::output_iterator_tag>,
		typename std::conditional <
		std::is_base_of<Tag0, CurrentMax>::value,
		type_define<Tag0>,
		maximum_iterator_catagory<CurrentMax, Tags...> >::type>::type>::type
	{};

	template<typename CurrentMax>
	struct maximum_iterator_catagory<CurrentMax>
	{
		typedef CurrentMax type;
	};

	template <typename... _Tags>
	using maximum_iterator_catagory_t = typename maximum_iterator_catagory<_Tags...>::type;

	template<typename... _Iterables>
	class zip_iterator {};

	template<typename _Ty, typename... _Iterables>
	class zip_iterator<_Ty, _Iterables...> : public std::iterator < maximum_iterator_catagory_t<typename std::iterator_traits<_Ty>::iterator_category, typename std::iterator_traits<_Iterables>::iterator_category...>,
		iterators_tuple_return_type_t < _Ty, _Iterables... >>
	{
		// the perfect forwarding will ensure that if we recieved rvalue refrence _Ty will by non-refrence and otherwise we have lvalue refrence
		using IteratorType = _Ty;
		using TupleReturnType = typename iterators_tuple_return_type<_Ty, _Iterables...>::type;
		using HeadDerefRetType = decltype(std::get<0>(std::declval<TupleReturnType>()));
		using ConstTupleReturnType = typename iterators_tuple_return_type<const _Ty, const _Iterables...>::type;
		using ConstHeadDerefRetType = decltype(std::get<0>(std::declval<ConstTupleReturnType>()));

	public:

		template<typename _Uy, typename... __Iterables,
			typename = std::enable_if_t <
			zip_test < std::is_convertible,
			types<_Uy&&, __Iterables&&...>,
			types < _Ty, _Iterables... >> ::value >>
			zip_iterator(_Uy&& head, __Iterables&&... tail) : // prevent copy-constructor confusion
			m_head_iterator(std::forward<_Uy>(head)),
			m_tail_iterator(std::forward<__Iterables>(tail)...)
		{
		}


		zip_iterator<_Ty, _Iterables...> & operator++() {
			++m_head_iterator;
			++m_tail_iterator;

			return *this;
		}

		template<typename _Uy>
		std::enable_if_t<std::is_lvalue_reference<_Uy>::value, std::reference_wrapper<std::remove_reference_t<_Uy>>>
			wrap_refrence_or_value(_Uy val) const
		{
			using ref_wrap_type = typename std::reference_wrapper<std::remove_reference_t<_Uy>>;
			return ref_wrap_type(val);
		}

		template<typename _Uy>
		std::enable_if_t<!std::is_reference<_Uy>::value, _Uy>
			wrap_refrence_or_value(_Uy val) const
		{
			return val;
		}


		TupleReturnType operator*()
		{
			return std::tuple_cat(std::make_tuple(wrap_refrence_or_value<decltype(*m_head_iterator)>(*m_head_iterator)), *m_tail_iterator);
		}

		ConstTupleReturnType operator*() const
		{
			return std::tuple_cat(std::make_tuple(wrap_refrence_or_value<decltype(*m_head_iterator)>(*m_head_iterator)), *m_tail_iterator);
		}

		bool operator==(const zip_iterator & other) const
		{
			if (m_head_iterator == other.m_head_iterator) {
				return true;
			}

			return m_tail_iterator == other.m_tail_iterator;
		}

		bool operator<(const zip_iterator & other) const
		{
			if (m_head_iterator < other.m_head_iterator) {
				return m_tail_iterator < other.m_tail_iterator;
			}

			return false;
		}

		bool operator!=(const zip_iterator & other) const
		{
			return !(*this == other);
		}

	private:
		// remove refrence to make it store the data internally
		IteratorType m_head_iterator;
		zip_iterator<_Iterables...> m_tail_iterator;
	};

	template<>
	class zip_iterator<> : public std::iterator < std::random_access_iterator_tag, std::tuple<> >
	{
		// TODO implement every possible iterator property
	public:
		zip_iterator<> & operator++() {
			return *this;
		}

		zip_iterator<> & operator++(int) {
			return *this;
		}

		std::tuple<> operator*() const
		{
			return std::tuple<>();
		}

		std::tuple<> * operator->()
		{
			static std::tuple<> empty;
			return &empty;
		}

		bool operator==(const zip_iterator<> & other) const
		{
			return false;
		}

		bool operator!=(const zip_iterator<> & other) const
		{
			return !(*this == other);
		}

		bool operator<(const zip_iterator<> & other) const
		{
			return true;
		}
	};

	template<typename... Iterables>
	zip_iterator<Iterables...> zip_iterators(Iterables&&... iters)
	{
		return zip_iterator<Iterables...>(std::forward<Iterables>(iters)...);
	}

}
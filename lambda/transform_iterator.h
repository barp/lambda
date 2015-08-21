#pragma once
#include <lambda/metautils.h>
#include <utility>

namespace lambda {
	template <typename Iter, typename Func>
	class transform_iterator : std::iterator<std::input_iterator_tag, std::remove_reference_t<decltype(*std::declval<Iter>())>>
	{
	public:
		template <typename _Iter, typename _Func>
		transform_iterator(_Iter&& iter, _Func&& func, std::enable_if_t<std::is_convertible<Iter, _Iter>::value && std::is_convertible<Func, _Func>::value>* = 0) :
			m_iterator(std::forward<_Iter>(iter)),
			m_functor(std::forward<_Func>(func))
		{
		}

		decltype(auto) operator*()
		{
			return m_functor(*m_iterator);
		}

		transform_iterator & operator++()
		{
			++m_iterator;
			return *this;
		}

		bool operator==(const transform_iterator & other) const
		{
			return m_iterator == other.m_iterator;
		}

		bool operator!=(const transform_iterator & other) const
		{
			return m_iterator != other.m_iterator;
		}

	private:
		Iter m_iterator;
		Func m_functor;
	};

	template <typename Iter, typename Func>
	transform_iterator<Iter, Func> make_transform_iterator(Iter&& iter, Func&& func)
	{
		return transform_iterator<Iter, Func>(std::forward<Iter>(iter), std::forward<Func>(func));
	}
}
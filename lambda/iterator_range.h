#pragma once
#include <utility>

namespace lambda {
	template <typename Iter1, typename Iter2>
	class iterator_range
	{
	public:
		template<typename _Iter1, typename _Iter2>
		iterator_range(_Iter1&& begin, _Iter2&& end, typename std::enable_if<std::is_convertible<Iter1, _Iter1>::value && std::is_convertible<Iter2, _Iter1>::value>::type* = 0) :
			m_begin(std::forward<_Iter1>(begin)),
			m_end(std::forward<_Iter2>(end))
		{

		}

		Iter1 & begin() { return m_begin; }
		Iter2 & end() { return m_end; }

	private:
		Iter1 m_begin;
		Iter2 m_end;
	};

	template<typename _Iter1, typename _Iter2>
	iterator_range<_Iter1, _Iter2> make_iterator_range(_Iter1&& begin, _Iter2&& end)
	{
		return iterator_range<_Iter1, _Iter2>(std::forward<_Iter1>(begin), std::forward<_Iter2>(end));
	}
}
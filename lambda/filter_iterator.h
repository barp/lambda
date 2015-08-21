#pragma once
#include <iterator>

namespace lambda {
	template <typename Iter, typename Iter2, typename Pred>
	class filter_iterator : std::iterator<std::input_iterator_tag, std::remove_reference_t<decltype(*std::declval<Iter>())>>
	{
		using ValueType = std::remove_const_t<std::remove_reference_t<decltype(*std::declval<Iter>())>>;
	public:
		template <typename _Iter, typename _Iter2, typename _Func>
		filter_iterator(_Iter&& iter, _Iter2&& end, _Func&& func,
			std::enable_if_t<std::is_convertible<Iter, _Iter>::value && std::is_convertible<Pred, _Func>::value && std::is_convertible<Iter2, _Iter2>::value>* = 0) :
			m_iterator(std::forward<_Iter>(iter)),
			m_end(end),
			m_pred(std::forward<_Func>(func))
		{
			if (m_iterator != end) {
				while (!m_pred(m_current_value = *m_iterator))
				{
					if (++m_iterator == m_end)
						break;
				}
			}
		}

		ValueType & operator*()
		{
			return m_current_value;
		}

		const ValueType & operator*() const
		{
			return m_current_value;
		}

		filter_iterator & operator++()
		{
			do {
				if (++m_iterator == m_end)
					break;
			} while (!m_pred(m_current_value = *m_iterator));
			return *this;
		}

		bool operator==(const filter_iterator & other) const
		{
			return m_iterator == other.m_iterator;
		}

		bool operator!=(const filter_iterator & other) const
		{
			return m_iterator != other.m_iterator;
		}

	private:
		Iter m_iterator;
		Iter2 m_end;
		mutable Pred m_pred; // the predicate can change everytime we use it
		ValueType m_current_value;
	};

	template <typename Iter, typename Iter2, typename Pred>
	filter_iterator<Iter, Iter2, Pred> make_filter_iterator(Iter&& iter, Iter2&& end, Pred&& func)
	{
		return filter_iterator<Iter, Iter2, Pred>(std::forward<Iter>(iter), std::forward<Iter2>(end), std::forward<Pred>(func));
	}
}
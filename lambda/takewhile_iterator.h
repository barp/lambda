#pragma once
#include <iterator>

namespace lambda {

	template <typename Iter, typename Pred>
	class takewhile_iterator : std::iterator<std::input_iterator_tag, std::remove_reference_t<decltype(*std::declval<Iter>())>>
	{
		using ValueType = decltype(*std::declval<Iter>());
	public:
		template <typename _Iter, typename _Func>
		takewhile_iterator(_Iter&& iter, _Func&& func,
			std::enable_if_t<std::is_convertible<Iter, _Iter>::value && std::is_convertible<Pred, _Func>::value>* = 0) :
			m_iterator(std::forward<_Iter>(iter)),
			m_pred(std::forward<_Func>(func)),
			computed_at_end(false)
		{
		}


		ValueType operator*()
		{
			return *m_iterator;
		}

		ValueType operator*() const
		{
			return *m_iterator;
		}

		takewhile_iterator & operator++()
		{
			++m_iterator;
			computed_at_end = false;
			return *this;
		}

		bool operator==(const takewhile_iterator & other) const
		{
			return m_iterator == other.m_iterator || (computed_at_end && at_end) || (!(computed_at_end = true) || (at_end = !m_pred(*m_iterator)));
		}

		bool operator!=(const takewhile_iterator & other) const
		{
			return !(*this == other);
		}

	private:
		Iter m_iterator;
		mutable Pred m_pred; // the predicate can change everytime we call it
		bool at_end;
		bool computed_at_end;
	};

	template <typename Iter, typename Pred>
	takewhile_iterator<Iter, Pred> make_takewhile_iterator(Iter&& iter, Pred&& func)
	{
		return takewhile_iterator<Iter, Pred>(std::forward<Iter>(iter), std::forward<Pred>(func));
	}
}
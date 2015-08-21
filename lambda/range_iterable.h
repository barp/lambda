#pragma once
#include <type_traits>

namespace lambda {
	template<typename IntegerType>
	class range_iterable
	{
		static_assert(std::is_integral<IntegerType>::value, "The type of the values must be integral type");

		class range_iterator : public std::iterator<std::input_iterator_tag, const IntegerType>
		{
		public:
			range_iterator(IntegerType start) :
				m_current(start)
			{}

			range_iterator & operator++() noexcept(noexcept(++m_current))
			{
				++m_current;
				return *this;
			}

			range_iterator operator++(int) noexcept(noexcept(m_current++))
			{
				range_iterator old(*this);
				m_current++;
				return old;
			}

			IntegerType operator*() const noexcept
			{
				return m_current;
			}

			bool operator==(const range_iterator & other) const noexcept(noexcept(m_current == other.m_current))
			{
				return m_current == other.m_current;
			}

			bool operator!=(const range_iterator & other) const noexcept(noexcept(*this == other))
			{
				return !(*this == other);
			}

		private:
			IntegerType m_current;
		};

	public:
		typedef range_iterator const_iterator;

		range_iterable(IntegerType start, IntegerType end);

		const_iterator begin() const;
		const_iterator end() const;

	private:
		IntegerType m_start;
		IntegerType m_end;
	};

	template<typename IntegerType>
	typename range_iterable<IntegerType>::const_iterator range_iterable<IntegerType>::end() const
	{
		return range_iterator(m_end);
	}

	template<typename IntegerType>
	typename range_iterable<IntegerType>::const_iterator range_iterable<IntegerType>::begin() const
	{
		return range_iterator(m_start);
	}

	template<typename IntegerType>
	range_iterable<IntegerType>::range_iterable(IntegerType start, IntegerType end) :
		m_start(start),
		m_end(end)
	{

	}


	template<typename IntegerType>
	range_iterable<IntegerType> range(IntegerType start, IntegerType end)
	{
		return range_iterable<IntegerType>(start, end);
	}

}
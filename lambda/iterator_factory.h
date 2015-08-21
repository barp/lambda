#include <lambda/zip_iterator.h>
#include <lambda/iterator_range.h>
#include <lambda/transform_iterator.h>
#include <lambda/filter_iterator.h>
#include <lambda/metautils.h>
#include <lambda/takewhile_iterator.h>

namespace lambda {
	template<typename Iterable, typename Func>
	decltype(auto) map(Iterable&& iter, Func&& func)
	{
		return make_iterator_range(lambda::make_transform_iterator(std::begin(std::forward<Iterable>(iter)), std::forward<Func>(func)),
			make_transform_iterator(std::end(std::forward<Iterable>(iter)), std::forward<Func>(func)));
	}

	template<typename ValueType, typename Func>
	decltype(auto) map(std::initializer_list<ValueType> iter, Func&& func)
	{
		return make_iterator_range(lambda::make_transform_iterator(std::begin(iter), std::forward<Func>(func)),
			make_transform_iterator(std::end(iter), std::forward<Func>(func)));
	}

	template<typename Iterable, typename Pred>
	decltype(auto) filter(Iterable&& iter, Pred&& func)
	{
		return make_iterator_range(make_filter_iterator(std::begin(std::forward<Iterable>(iter)), std::end(std::forward<Iterable>(iter)), std::forward<Pred>(func)),
			make_filter_iterator(std::end(std::forward<Iterable>(iter)), std::end(std::forward<Iterable>(iter)), std::forward<Pred>(func)));
	}

	template<typename ValueType, typename Pred>
	decltype(auto) filter(std::initializer_list<ValueType> iter, Pred&& func)
	{
		return filter<std::initializer_list<ValueType>, Pred>(std::move(iter), std::forward<Pred>(func));
	}

	template<typename Iterable, typename Pred>
	decltype(auto) takewhile(Iterable&& iter, Pred&& func)
	{
		return make_iterator_range(make_takewhile_iterator(std::begin(std::forward<Iterable>(iter)), std::forward<Pred>(func)),
			make_takewhile_iterator(std::end(std::forward<Iterable>(iter)), std::forward<Pred>(func)));
	}

	template<typename ValueType, typename Pred>
	decltype(auto) takewhile(std::initializer_list<ValueType> iter, Pred&& func)
	{
		return takewhile<std::initializer_list<ValueType>, Pred>(std::move(iter), std::forward<Pred>(func));
	}

	template<typename Iterable, typename IntegralType>
	decltype(auto) take(Iterable&& iter, IntegralType&& count)
	{
		return takewhile(std::forward<Iterable>(iter), [i = IntegralType(), c = std::forward<IntegralType>(count)](const auto& x) mutable { return i++ < c; });
	}

	template<typename ValueType, typename IntegralType>
	decltype(auto) take(std::initializer_list<ValueType> iter, IntegralType&& count)
	{
		return take<std::initializer_list<ValueType>, IntegralType>(std::move(iter), std::forward<IntegralType>(count));
	}
	
	template<typename... Iterables>
	decltype(auto) zip(Iterables&&... iters)
	{
		return make_iterator_range(zip_iterators(std::begin(std::forward<Iterables>(iters))...),
			zip_iterators(std::end(std::forward<Iterables>(iters))...));
	}

	template<typename _Ty, typename _Func>
	bool all(_Ty&& iterable, _Func&& functor)
	{
		return std::all_of(std::begin(std::forward<_Ty>(iterable)), std::end(std::forward<_Ty>(iterable)), std::forward<_Func>(functor));
	}

	template<typename _Ty, typename _Func>
	bool any(_Ty&& iterable, _Func&& functor)
	{
		return std::any_of(std::begin(std::forward<_Ty>(iterable)), std::end(std::forward<_Ty>(iterable)), std::forward<_Func>(functor));
	}

}
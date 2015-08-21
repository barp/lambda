#pragma once

#define DEFINE_TAGGED_FIELD(type, name) \
	struct name##_tag { \
	}; \
	tagged_field<type> name; \
	template <typename _Uy> \
	void __set_value(name##_tag tag, _Uy&& value) { \
		name.value = std::forward<_Uy>(value); \
	}

namespace lambda {
	template <typename _Ty>
	struct tagged_field
	{
		typedef _Ty type;
		T value;
	};

	template <typename... Args>
	struct set_values;

	template <typename Owner, typename Tag, typename ValueType, typename... Args>
	struct set_values<Owner, Tag, ValueType, Args...>
	{
		set_values(Owner & self) :
			m_self(self)
		{

		}

		void operator()(Tag tag, ValueType value, Args... args)
		{
			self.__set_value(tag, value);
			(set_values<Owner, Args...>(self))(args...);
		}

		Owner & self;
	};

	template <typename Owner>
	struct set_values<Owner>
	{
		set_values(Owner & self) :
			m_self(self)
		{

		}

		void operator()()
		{

		}

		Owner & self;
	};

}

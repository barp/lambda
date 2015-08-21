# lambda
a C++14 iterators and functional programming library


example code
```c++
	for (auto x : lambda::map(lambda::filter({ 1, 2, 3, 4, 5, 6, 7 }, [](auto x) { return x % 2 == 0; }), [](auto x) { return x / 2; }))
	{
		std::cout << x << std::endl;
	}

	std::cout << "Second Part" << std::endl;

	for (auto y : lambda::filter(lambda::map({ 1, 2, 3, 4 }, [](auto x) { return x * x; }), [](auto x) { return x % 2 == 0; }))
	{
		std::cout << y << std::endl;
	}

	std::cout << "Third Part" << std::endl;
	for (auto y : lambda::take(lambda::map({ 1, 2, 3, 4 }, [](auto x) { return x * x; }), 2))
	{
		std::cout << y << std::endl;
	}
	
	std::cout << "Fourth Part" << std::endl;
	for (auto y : lambda::takewhile(lambda::map({ 1, 2, 3, 4 }, [](auto x) { return x * x; }), [](auto x) { return x % 2 != 0; }))
	{
		std::cout << y << std::endl;
	}
```

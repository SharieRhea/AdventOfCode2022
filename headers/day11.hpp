#ifndef DAY11
#define DAY11

#include <functional>
#include <queue>
class Monkey {
public:
	std::queue<long> items;
	std::function<long(long)> inspect;
	std::function<int(long)> test;

	long itemsInspected = 0;

	Monkey(std::queue<long> items, std::function<long(long)> const& inspect, std::function<int(long)> const& test) {
		this->items = items;
		this->inspect = inspect;
		this->test = test;
	}

	// copy constructor
	Monkey(Monkey *original) {
		this->items = original->items;
		this->inspect = original->inspect;
		this->test = original->test;
	}
};

#endif

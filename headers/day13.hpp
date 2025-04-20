#ifndef DAY13
#define DAY13

#include <vector>

class ListItem {
public:
	int number;
	std::vector<ListItem*> list;

	ListItem(int number) {
		this->number = number;
	}
};

#endif

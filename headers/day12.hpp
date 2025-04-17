#ifndef DAY12
#define DAY12

class Node {
public:
	int x;
	int y;
	int steps;

	Node(int x, int y) {
		this->x = x;
		this->y = y;
		this->steps = 0;
	}

	Node(int x, int y, int steps) {
		this->x = x;
		this->y = y;
		this->steps = steps;
	}
};

#endif

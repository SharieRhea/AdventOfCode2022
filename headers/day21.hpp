#ifndef DAY21
#define DAY21

#include <string>

class Monkey {
public:
	long value = -1;
	std::string left = "";
	char op = '+';
	std::string right = "";

	Monkey(const std::string *left, char op, const std::string *right) {
		this->left = *left;
		this->op = op;
		this->right = *right;
	}

	explicit Monkey(int value) {
		this->value = value;
	}
};

#endif

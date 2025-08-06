#include "headers/day21.hpp"
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

std::map<std::string, Monkey*> map;

// recursively work to figure out what number this monkey will yell
long yellNumber(Monkey *monkey) {
	if (monkey->left.empty())
		return monkey->value;

	long left = yellNumber(map[monkey->left]);
	long right = yellNumber(map[monkey->right]);

	switch (monkey->op) {
		case '+':
			return left + right;
		case '-':
			return left - right;
		case '*':
			return left * right;
		case '/':
			return left / right;
	}
	return 0;
}

long part2() {
	// start somewhere pretty big
	map["humn"]->value = 1e11;
	int direction = 1;
	long difference = 0;
	long step = 1e11;

	long left = yellNumber(map[map["root"]->left]);
	long right = yellNumber(map[map["root"]->right]);
	while (left != right) {

		if (labs(left - right) > difference) {
			// we got further away than before, reverse and reduce step size
			direction *= -1;
			step = std::max(1l, step / 2);
		}
		difference = labs(left - right);
		map["humn"]->value += (step * direction);

		left = yellNumber(map[map["root"]->left]);
		right = yellNumber(map[map["root"]->right]);
	}
	return map["humn"]->value;
}

int main() {
	std::ifstream file("resources/day21.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	std::string line;
	while(std::getline(file, line)) {
		std::string name = line.substr(0, 4);
		if (std::isdigit(line[6])) {
			int value = std::stoi(line.substr(6));
			map[name] = new Monkey(value);
		}
		else {
			std::string left = line.substr(6, 4);
			std::string right = line.substr(13, 4);
			char op = line[11];
			map[name] = new Monkey(&left, op, &right);
		}
	}

	std::cout << "Part 1: " << yellNumber(map["root"]) << "\n";
	std::cout << "Part 2: " << part2() << "\n";
}

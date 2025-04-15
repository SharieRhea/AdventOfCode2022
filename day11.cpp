#include <algorithm>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits.h>
#include <string>
#include <vector>
#include "headers/day11.hpp"

int primeProduct = 1;

Monkey* parseMonkey(std::ifstream *file) {
	std::string line;
	// first line just has the monkey number
	std::getline(*file, line);

	// parse list of starting items
	std::queue<long> items;
	std::getline(*file, line);
	line = line.substr(18);
	while (line.find(", ") != -1) {
		items.push(std::stoi(line.substr(0, line.find(", "))));
		line = line.substr(line.find(", ") + 2);
	}
	items.push(std::stoi(line));

	// parse the inspect operation
	std::getline(*file, line);
	char operation = line[23];
	bool useCurrentValue = false;
	int number = 0;
	if (line[25] == 'o')
		useCurrentValue = true;
	else 
		number = std::stoi(line.substr(25));

	std::function<long(long)> inspect = [=](long item) -> long {
		long rightSide = number;
		if (useCurrentValue)
			rightSide = item;
		return (operation == '+') ? item + rightSide : item * rightSide;
	};

	// parse the test operation
	std::getline(*file, line);
	int testNumber = std::stoi(line.substr(21));
	// for part 2, find the product of all test values to reduce worry
	primeProduct *= testNumber;

	// parse which monkeys to throw to
	std::getline(*file, line);
	int trueCase = std::stoi(line.substr(29));
	std::getline(*file, line);
	int falseCase = std::stoi(line.substr(30));

	std::function<int(long)> test = [=](long item) -> int {
		return (item % testNumber == 0) ? trueCase : falseCase;
	};
	
	// remove the empty line between monkeys
	std::getline(*file, line);

	return new Monkey(items, inspect, test);
}

bool compareMonkeys(Monkey *one, Monkey *two) {
	return one->itemsInspected > two->itemsInspected;
}

int main() {
	std::ifstream file("resources/day11.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	std::vector<Monkey*> monkeys1;
	std::vector<Monkey*> monkeys2;
	while (file.peek() != EOF) {
		monkeys1.push_back(parseMonkey(&file));	
		Monkey *copy = new Monkey(monkeys1.back());
		monkeys2.push_back(copy);
	}

	// part 1, 20 rounds
	for (int i = 0; i < 20; i++) {
		for (auto monkey = monkeys1.begin(); monkey != monkeys1.end(); monkey++) {
			while (!(*monkey)->items.empty()) {
				int item = (*monkey)->items.front();
				(*monkey)->items.pop();
				int result = (*monkey)->inspect(item);
				result /= 3;
				monkeys1[(*monkey)->test(result)]->items.push(result);
				(*monkey)->itemsInspected++;
			}
		}
	}

	// part 2, 10000 rounds
	for (int i = 0; i < 10000; i++) {
		for (auto monkey = monkeys2.begin(); monkey != monkeys2.end(); monkey++) {
			while (!(*monkey)->items.empty()) {
				long item = (*monkey)->items.front();
				(*monkey)->items.pop();
				long result = (*monkey)->inspect(item);
				result %= primeProduct;
				monkeys2[(*monkey)->test(result)]->items.push(result);
				(*monkey)->itemsInspected++;
			}
		}
	}

	std::sort(monkeys1.begin(), monkeys1.end(), compareMonkeys);
	std::sort(monkeys2.begin(), monkeys2.end(), compareMonkeys);
	
	std::cout << "Part 1: " << monkeys1[0]->itemsInspected * monkeys1[1]->itemsInspected << "\n";
	std::cout << "Part 2: " << monkeys2[0]->itemsInspected * monkeys2[1]->itemsInspected << "\n";

	return 0;
}

#include "headers/day20.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sched.h>
#include <string>
#include <vector>

std::vector<MappedNumber> originalOrder;
std::vector<MappedNumber> mixed;

int size;

int find(MappedNumber number) {
	for (int i = 0; i < size; i++) {
		// use the number's original index to make sure we are moving *that* one
		if (mixed[i].value == number.value && mixed[i].originalIndex == number.originalIndex)
			return i;
	}
	return -1;
}

void mix(MappedNumber number) {
	// find the index of this number in the shuffled list
	int originalIndex = find(number);
	mixed.erase(mixed.begin() + originalIndex);
	// mod to account for wrapping using size WITHOUT this number included
	int newIndex = (originalIndex + number.value) % (size - 1);
	// make negative indices positive
	if (newIndex < 0)
		newIndex += size - 1;
	mixed.insert(mixed.begin() + newIndex, number);
}

int64_t getGrooves() {
	int zeroIndex = -1; for (int i = 0; i < size; i++) {
		if (mixed[i].value == 0) {
			zeroIndex = i;
			break;
		}
	}

	int64_t grooves = 0;
	for (int i = 1000; i < 3001; i += 1000) {
		grooves += mixed[(zeroIndex + i) % size].value;
	}
	return grooves;
}

int main() {
	std::ifstream file("resources/day20.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	std::string line;
	int index = 0;

	while (std::getline(file, line)) {
		int value = std::stoi(line);
		originalOrder.push_back(MappedNumber(value, index));
		mixed.push_back(MappedNumber(value, index));
		index++;
	}
	size = originalOrder.size();

	// part 1
	for (auto number : originalOrder) {
		mix(number);
	}
	std::cout << "Part 1: " << getGrooves() << "\n";

	// part 2
	mixed.clear();
	for (auto number : originalOrder) {	
		// multiply each by the decryption key
		mixed.push_back(MappedNumber(number.value * 811589153, number.originalIndex));
	}
	originalOrder = mixed;
	// mix 10 times
	for (int i = 0; i < 10; i++) {
		for (auto number : originalOrder) {
			mix(number);
		}
	}
	std::cout << "Part 2: " << getGrooves() << "\n";
}

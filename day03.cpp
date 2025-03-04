#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

char getDuplicateItem(std::string contents) {
	int left = 0;
	int right = contents.length() - 1;
	std::unordered_set<char> leftItems = {};
	std::unordered_set<char> rightItems = {};

	// using two pointers approach, built sets for each compartment
	while (left <= right) {
		leftItems.insert(contents[left]);
		rightItems.insert(contents[right]);
		left++;
		right--;
	}

	// compare the two sets and determine which element is present in both
	for (char item : leftItems) {
		if (rightItems.find(item) != leftItems.end())
			return item;
	}

	// no common element, should not happen if input is nice
	std::cerr << "No common item in rucksack!";
	exit(1);
}

char getCommonBadge(std::string bagOne, std::string bagTwo, std::string bagThree) {
	for (int i = 0; i < bagOne.length(); i++) {
		if (bagTwo.find(bagOne[i]) != -1 && bagThree.find(bagOne[i]) != -1)
			return bagOne[i];
	}

	// no common element, should not happen if input is nice
	std::cerr << "No common badge in group!";
	exit(1);
}

int part1() {
	std::ifstream file("resources/day03.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	std::string line;
	int priorities = 0;

	while (getline(file, line)) {
		char commonItem = getDuplicateItem(line);
		// a-z is 1-26, A-Z is 27-52
		if (commonItem > 90)
			priorities += commonItem - 96;
		else
			priorities += commonItem - 38;
	}
	file.close();

	return priorities;
}

int part2() {
	std::ifstream file("resources/day03.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	std::string line;
	std::vector<std::string> lines;

	while (getline(file, line)) {
		lines.push_back(line);
	}
	file.close();

	int priorities = 0;
	for (int i = 0; i < lines.size(); i += 3) {
		char badge = getCommonBadge(lines[i], lines[i + 1], lines[i + 2]);
		// a-z is 1-26, A-Z is 27-52
		if (badge > 90)
			priorities += badge - 96;
		else
			priorities += badge - 38;
	}

	return priorities;
}

int main() {
	std::cout << "Part 1: " << part1() << "\n";
	std::cout << "Part 2: " << part2() << "\n";
	return 0;
}

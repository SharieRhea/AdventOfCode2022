#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool comparator(int a, int b) {
	return a > b;
}

int main() {
	std::ifstream file("resources/day01.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	std::string line;
	int calorie_sum = 0;
	std::vector<int> sums;

	while (getline(file, line)) {
		// empty line means moving to a new elf, add to list and reset calories
		if (line.empty()) {
			sums.push_back(calorie_sum);
			calorie_sum = 0;
			continue;
		}
		// convert to int and add to the running total for this elf
		int calories = std::stoi(line);
		calorie_sum += calories;
	}
	file.close();

	// sort greatest to least
	std::sort(sums.begin(), sums.end(), comparator);

	// max calorie sum
	std::cout << "Part 1: " << sums[0] << "\n";
	// sum of three highest calorie sums
	std::cout << "Part 2: " << sums[0] + sums[1] + sums[2] << "\n";

	return 0;
}

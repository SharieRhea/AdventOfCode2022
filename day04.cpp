#include <fstream>
#include <iostream>
#include <string>

bool contains(int lower1, int upper1, int lower2, int upper2) {
	// group1 is fully contained by group2
	if (lower1 >= lower2 && upper1 <= upper2)
		return true;
	// group2 is fully contained by group2
	if (lower2 >= lower1 && upper2 <= upper1)
		return true;

	return false;
}

bool overlaps(int lower1, int upper1, int lower2, int upper2) {
	// beginning of group2 overlaps with end of group1
	if (lower1 <= lower2 && upper1 >= lower2)
		return true;
	// beginning of group1 overlaps with end of group2
	if (lower2 <= lower1 && upper2 >= lower1)
		return true;

	return false;
}

int main() {
std::ifstream file("resources/day04.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	int containedGroups = 0;
	int overlappingGroups = 0;
	std::string line;
	while (getline(file, line)) {
		int commaPosition = line.find(",");
		std::string group1 = line.substr(0, commaPosition);
		std::string group2 = line.substr(commaPosition + 1);

		int dashPosition = group1.find("-");
		int lower1 = std::stoi(group1.substr(0, dashPosition));
		int upper1 = std::stoi(group1.substr(dashPosition + 1));
		dashPosition = group2.find("-");
		int lower2 = std::stoi(group2.substr(0, dashPosition));
		int upper2 = std::stoi(group2.substr(dashPosition + 1));

		if (contains(lower1, upper1, lower2, upper2))
			containedGroups++;
		if (overlaps(lower1, upper1, lower2, upper2))
			overlappingGroups++;
	}
	file.close();

	std::cout << "Part 1: " << containedGroups << "\n";
	std::cout << "Part 2: " << overlappingGroups << "\n";

	return 0;
}

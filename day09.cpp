#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>

// mapping for directional moves
std::map<char, std::pair<int, int>> directionVectors = {
	{'U', std::pair<int, int>(0, 1)},
	{'D', std::pair<int, int>(0, -1)},
	{'L', std::pair<int, int>(-1, 0)},
	{'R', std::pair<int, int>(1, 0)}
};

// mapping from difference head(x, y) - tail (x, y) to tail's movement vector
std::map<std::pair<int, int>, std::pair<int, int>> catchUpVectors = {
	// overlapping
	{std::pair<int, int>(0, 0), std::pair<int, int>(0, 0)},

	// straight moves
	{std::pair<int, int>(2, 0), std::pair<int, int>(1, 0)},
	{std::pair<int, int>(-2, 0), std::pair<int, int>(-1, 0)},
	{std::pair<int, int>(0, 2), std::pair<int, int>(0, 1)},
	{std::pair<int, int>(0, -2), std::pair<int, int>(0, -1)},

	// diagonal moves
	{std::pair<int, int>(2, 1), std::pair<int, int>(1, 1)},
	{std::pair<int, int>(2, -1), std::pair<int, int>(1, -1)},
	{std::pair<int, int>(-2, 1), std::pair<int, int>(-1, 1)},
	{std::pair<int, int>(-2, -1), std::pair<int, int>(-1, -1)},
	{std::pair<int, int>(1, 2), std::pair<int, int>(1, 1)},
	{std::pair<int, int>(-1, 2), std::pair<int, int>(-1, 1)},
	{std::pair<int, int>(1, -2), std::pair<int, int>(1, -1)},
	{std::pair<int, int>(-1, -2), std::pair<int, int>(-1, -1)},

	// corner moves
	{std::pair<int, int>(2, 2), std::pair<int, int>(1, 1)},
	{std::pair<int, int>(2, -2), std::pair<int, int>(1, -1)},
	{std::pair<int, int>(-2, 2), std::pair<int, int>(-1, 1)},
	{std::pair<int, int>(-2, -2), std::pair<int, int>(-1, -1)},

};

int main() {
	std::ifstream file("resources/day09.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	// for part 1, head and tail both start at (0, 0)
	std::pair<int, int> head = std::pair<int, int>(0, 0);
	std::pair<int, int> tail = std::pair<int, int>(0, 0);
	// keep track of the tail's visited positions
	std::unordered_set<std::string> part1;

	// for part 2, all knots start at (0, 0)
	std::pair<int, int> knots[10];
	std::unordered_set<std::string> part2;

	std::stringstream key;
	key << tail.first << " " << tail.second;
	part1.insert(key.str());
	part2.insert(key.str());

	std::string line;
	while (std::getline(file, line)) {
		char direction = line[0];
		int repeats = std::stoi(line.substr(2));

		for (int i = 0; i < repeats; i++) {
			// part 1
			head.first += directionVectors[direction].first;
			head.second += directionVectors[direction].second;
			// find the difference between head's position and tail's position
			std::pair<int, int> difference = std::pair<int, int>(head.first - tail.first, head.second - tail.second);
			tail.first += catchUpVectors[difference].first;
			tail.second += catchUpVectors[difference].second;
			// add the new tail position to the set
			std::stringstream key1;
			key1 << tail.first << " " << tail.second;
			part1.insert(key1.str());

			// part 2
			knots[0].first += directionVectors[direction].first;
			knots[0].second += directionVectors[direction].second;
			// allow the rest of the rope to catch up
			for (int knot = 0; knot < 9; knot++) {
				// find the difference between this knot's position and the next knot down 
				std::pair<int, int> difference = std::pair<int, int>(
						knots[knot].first - knots[knot + 1].first, knots[knot].second - knots[knot + 1].second);
				knots[knot + 1].first += catchUpVectors[difference].first;
				knots[knot + 1].second += catchUpVectors[difference].second;
			}
			// add the new tail position to the set
			std::stringstream key2;
			key2 << knots[9].first << " " << knots[9].second;
			part2.insert(key2.str());
		}
	}

	std::cout << "Part 1: " << part1.size() << "\n";
	std::cout << "Part 2: " << part2.size() << "\n";

	return 0;
}

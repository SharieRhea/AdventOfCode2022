#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

std::string line;
std::unordered_set<std::string> map;
// keep track of the lowest rock so we know where the abyss starts
int lowest = 0;

std::string makeKey(int x, int y) {
	std::stringstream key;
	key << x << " " << y;
	return key.str();
}

std::vector<std::pair<int, int>> parseRock() {
	std::vector<std::pair<int, int>> points;
	while (line.find('>') != -1) {
		// grab the first number, discard plus comma, grab second
		int x = std::stoi(line);
		line = line.substr(line.find(',') + 1);
		int y = std::stoi(line);
		points.push_back(std::pair<int, int>(x, y));

		if (y > lowest)
			lowest = y;

		// discard the arrow
		line = line.substr(line.find('>') + 1);
	}
	// grab the last point
	int x = std::stoi(line);
	line = line.substr(line.find(',') + 1);
	int y = std::stoi(line);
	points.push_back(std::pair<int, int>(x, y));
	
	if (y > lowest)
		lowest = y;

	return points;
}

void placeRocks(std::vector<std::pair<int, int>> points) {
	// form lines of rocks between each pair of points
	std::pair<int, int> left = points[0];

	for (int i = 1; i < points.size(); i++) {
		std::pair<int, int> right = points[i];

		if (left.first != right.first) {
			// horizontal movement
			int direction = (right.first - left.first > 0) ? 1 : -1; 
			for (int i = left.first; i != right.first; i += direction) {
				map.insert(makeKey(i, left.second));		
			}
			map.insert(makeKey(right.first, left.second));
		}
		else {
			// vertical movement
			int direction = (right.second - left.second > 0) ? 1 : -1; 
			for (int i = left.second; i != right.second; i += direction) {
				map.insert(makeKey(left.first, i));		
			}
			map.insert(makeKey(left.first, right.second));
		}
		// prepare for the next line
		left = points[i];
	}
}

bool simulateFall(int part) {
	// all sand starts falling from (500,0)
	int x = 500;
	int y = 0;

	while (y < lowest + 1) {
		// try to move straight down first
		if (map.find(makeKey(x, y + 1)) == map.end()) {
			y++;
			continue;
		}
		// diagonal left
		if (map.find(makeKey(x - 1, y + 1)) == map.end()) {
			x--;
			y++;
			continue;
		}
		// diagonal right
		if (map.find(makeKey(x + 1, y + 1)) == map.end()) {
			x++;
			y++;
			continue;
		}
		// cannot move any further, add to map as "settled"
		map.insert(makeKey(x, y));
		return true;
	}
	// falling into abyss OR hit the floor
	if (part == 2)
		map.insert(makeKey(x, y));
	return false;
}

int main() {
	std::ifstream file("resources/day14.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	while (std::getline(file, line)) {
		placeRocks(parseRock());
	}

	int grains = 0;
	while (simulateFall(1)) {
		grains++;
	}
	std::cout << "Part 1: " << grains << "\n";

	std::string source = makeKey(500, 0);
	while (map.find(source) == map.end()) {
		simulateFall(2);
		grains++;
	}
	std::cout << "Part 2: " << grains << "\n";
}

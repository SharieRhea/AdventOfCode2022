#include "headers/day17.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

std::unordered_set<Point, PointHash> occupied;
std::unordered_set<Point, PointHash> current;

// keep track of the highest y level
int top = 0;
int windIndex = 0;
std::string line;

/*
 * rock 0: ####   rock 3: #
 *                        #
 * rock 1: .#.            #  
 *         ###            # 
 *         .#.            
 *                rock 4: ##
 * rock 2: ..#            ##
 *         ..#
 *         ###
 */
std::unordered_set<Point, PointHash> getRock(int index) {
	switch (index) {
		case 0:
			return { Point(2, top + 4), Point(3, top + 4), Point(4, top + 4), Point(5, top + 4) };
		case 1:
			return { Point(3, top + 6), Point(2, top + 5), Point(3, top + 5), Point(4, top + 5), Point(3, top + 4) };
		case 2:
			return { Point(4, top + 6), Point(4, top + 5), Point(2, top + 4), Point(3, top + 4), Point(4, top + 4) };
		case 3:
			return { Point(2, top + 7), Point(2, top + 6), Point(2, top + 5), Point(2, top + 4) };
		case 4:
			return { Point(2, top + 5), Point(3, top + 5), Point(2, top + 4), Point(3, top + 4) };
		default:
			std::cerr << "Invalid rock type!\n";
			exit(1);
	}
}

bool outOfBounds(const Point point) {
	return point.x < 0 || point.x > 6 || occupied.find(point) != occupied.end();
}

bool comparePoints(Point point1, Point point2) {
	if (point1.y == point2.y)
		return point1.x < point2.x;
	return point1.y < point2.y; 
}

void tryMoveHorizontal(int direction) {
	std::unordered_set<Point, PointHash> moved;
	for (Point point : current) {
		moved.insert(Point(point.x + direction, point.y));
	}

	// check if we've stayed within of bounds
	if (std::find_if(moved.begin(), moved.end(), outOfBounds) == moved.end())
		current = moved;
}

bool tryMoveDown() {
	std::unordered_set<Point, PointHash> moved;
	for (auto point : current) {
		moved.insert(Point(point.x, point.y - 1));
	}
	// see if any of moved is in occupied
	for (Point point : moved) {
		if (occupied.find(point) != occupied.end())
			return false;
	}
	current = moved;
	return true;
}

int part1(int blocks) {
	// set up the "floor"
	for (int x = 0; x < 7; x++) {
		occupied.insert(Point(x, 0));
	}

	for (int block = 0; block < blocks; block++) {
		current = getRock(block % 5);
		// alternate between moving the rock horizontally and then downwards
		while (true) {
			int direction = (line[windIndex] == '<') ? -1 : 1;
			windIndex = (windIndex != line.length() - 1) ? windIndex + 1 : 0;
			tryMoveHorizontal(direction);
			if (!tryMoveDown())
				break;
		}
		// update the occupied set and spawn a new block
		occupied.insert(current.begin(), current.end());
		// update the top level before spawning the next block
		top = std::max_element(occupied.begin(), occupied.end(), comparePoints)->y;
	}

	// reset 
	current.clear();
	occupied.clear();
	windIndex = 0;
	int returnValue = top;
	top = 0;
	return returnValue;
}

// return a string representing the top 5 rows of the tower to characterize the surface
std::string hashTopRows() {
	std::vector<Point> tower(occupied.begin(), occupied.end());
	std::vector<Point> points;
	std::copy_if(tower.begin(), tower.end(), std::back_inserter(points), [](Point point){return top - point.y < 5;});
	// sort to ensure we always get the same result
	std::sort(points.begin(), points.end(), comparePoints);
	std::stringstream key;
	for (auto point : points) {
		key << point.x << "," << top - point.y << ";";
	}
	return key.str();
}

long part2(long totalBlocks) {
	// cycle detection
	// total height = beforePattern + (repeatHeight * repeats) + afterPattern
	
	// create a key from the current TYPE of block, windIndex, and top 5 rows
	// value is the FIRST height we see for this key
	std::map<std::string, int> map;
	int repeatLength;
	int repeatHeight = 0;
	int firstDetectionBlock = 0;
	
	// set up the "floor"
	for (int x = 0; x < 7; x++) {
		occupied.insert(Point(x, 0));
	}

	int block = 0;
	while (true) {
		current = getRock(block % 5);
		// alternate between moving the rock horizontally and then downwards
		while (true) {
			int direction = (line[windIndex] == '<') ? -1 : 1;
			windIndex = (windIndex != line.length() - 1) ? windIndex + 1 : 0;
			tryMoveHorizontal(direction);
			if (!tryMoveDown())
				break;
		}
		// update the occupied set and spawn a new block
		occupied.insert(current.begin(), current.end());
		// update the top level before spawning the next block
		top = std::max_element(occupied.begin(), occupied.end(), comparePoints)->y;

		// cycle detection section
		std::stringstream key;
		key << block % 5 << ";" << windIndex << ";" << hashTopRows();

		// check to see if the key is a duplicate
		if (map.find(key.str()) != map.end()) {
			if (repeatHeight == 0) {
				// this is the first duplicate, the height change gives us the repeat height
				repeatHeight = top - map[key.str()];
				// this will be used to figure out how many blocks fall BEFORE the pattern starts
				firstDetectionBlock = block;
			}
			else if (repeatHeight != top - map[key.str()]) {
				// we're hitting the second cycle, now we can figure out how many blocks fall per cycle
				repeatLength = block - firstDetectionBlock;
				break;
			}
		}
		else
			map.insert({ key.str(), top});
		block++;
	}
	
	// clear before using the part1 method to find the beforeAndAfter height
	current.clear();
	occupied.clear();
	windIndex = 0;
	top = 0;

	// how many blocks fall before the pattern starts 
	int beforePatternBlock = firstDetectionBlock - repeatLength;
	long totalRepeats = (totalBlocks - beforePatternBlock) / repeatLength;
	int afterPatternBlock = totalBlocks - (totalRepeats * repeatLength) - beforePatternBlock;
	// total height of sections before and after the cycle repeats
	int beforeAndAfter = part1(beforePatternBlock + afterPatternBlock);
	long pattern = totalRepeats * repeatHeight;

	return pattern + beforeAndAfter;
}

int main() {
	std::ifstream file("resources/day17.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	std::getline(file, line);

	int height = part1(2022);
	long height2 = part2(1000000000000);

	std::cout << "Part 1: " << height << "\n";
	std::cout << "Part 2: " << height2 << "\n";
}

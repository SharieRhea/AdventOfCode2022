#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "headers/day15.hpp"

std::string line;
int beaconsOnLine;
std::unordered_set<std::string> beacons;
std::vector<Sensor*> sensors;
// the row we care about for part 1
int row = 2000000;

std::string makeKey(int x, int y) { 
	std::stringstream key;
	key << x << " " << y;
	return key.str();
}

void parseSensor() {
	line = line.substr(line.find("=") + 1);
	int sensorX = std::stoi(line);
	line = line.substr(line.find("=") + 1);
	int sensorY = std::stoi(line);
	line = line.substr(line.find("=") + 1);
	int beaconX = std::stoi(line);
	line = line.substr(line.find("=") + 1);
	int beaconY = std::stoi(line);

	// determine manhattan distance
	int distance = abs(sensorX - beaconX) + abs(sensorY - beaconY);

	sensors.push_back(new Sensor(sensorX, sensorY, distance));
	// keep track of beacons in the row we care about for part 1
	if (beaconY == row && beacons.find(makeKey(beaconX, beaconY)) == beacons.end())
		beaconsOnLine++;
	beacons.insert(makeKey(beaconX, beaconY));
}

// returns the x coords of the range covered by this sensor on the given y
std::pair<int, int> getRange(int y, Sensor* sensor) {
	int horizontalDistance = sensor->distance - abs(sensor->y - y);
	if (horizontalDistance < 0)
		return std::pair<int, int>(0, 0);
	return std::pair<int, int>(sensor->x - horizontalDistance, sensor->x + horizontalDistance);
}

// return a list of points just outside this sensors range
std::vector<std::pair<int, int>> getPoints(Sensor* sensor) {
	std::vector<std::pair<int, int>> returnValue;
	for (int offset = 0; offset < sensor->distance + 1; offset++) {
		int inverseOffset = sensor->distance + 1 - offset;
		returnValue.push_back(std::pair<int, int>(sensor->x + offset, sensor->y + inverseOffset));
		returnValue.push_back(std::pair<int, int>(sensor->x + inverseOffset, sensor->y - offset));
		returnValue.push_back(std::pair<int, int>(sensor->x - offset, sensor->y - inverseOffset));
		returnValue.push_back(std::pair<int, int>(sensor->x - inverseOffset, sensor->y + offset));
	}
	return returnValue;
}

bool withinRange(int x, int y, Sensor* sensor) {
	int distance = abs(x - sensor->x) + abs(y - sensor->y);
	return distance <= sensor->distance;
}

int main() {
	std::ifstream file("resources/day15.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}
	
	while (std::getline(file, line)) {
		parseSensor();
	}

	// part 1, for each sensor, get their ranges for the given row
	std::vector<std::pair<int, int>> ranges;
	for (auto sensor : sensors) {
		auto range = getRange(row, sensor);
		if (range.first != 0 && range.second != 0)
			ranges.push_back(range);
	}
	std::sort(ranges.begin(), ranges.end());

	// continue trying to merge ranges until nothing merges anymore
	std::vector<std::pair<int, int>> result;
	auto range = ranges.begin();
	auto current = *(range)++;
	while (range != ranges.end()) {
		if (current.second >= range->first)
			current.second = std::max(current.second, range->second);
		else {
			result.push_back(current);
			current = *range;
		}
		range++;
	}
	result.push_back(current);

	long positions = 0;
	for (auto range : result) {
		positions += range.second - range.first + 1;
	}

	std::pair<long, long> distress = std::pair<int, int>(-1, -1);
	// part 2, check each sensors border to find a spot where no sensor can reach
	for (auto sensor : sensors) {
		if (distress.first != -1)
			break;
		auto points = getPoints(sensor);
		for (auto point : points) {
			// don't check points out of range
			if (point.first < 0 || point.second < 0 || point.first > 4000000 || point.second > 4000000)
				continue;
			bool found = false;
			for (auto sensorToCheck : sensors) {
				if (withinRange(point.first, point.second, sensorToCheck)) {
					found = true;
					break;
				}
			}
			if (!found)
				distress = point;
		}
	}
	std::cout << "Part 1: " << positions - beaconsOnLine << "\n";
	std::cout << "Part 2: " << distress.first * 4000000 + distress.second << "\n";
}

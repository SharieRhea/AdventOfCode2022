#include "headers/day18.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

std::unordered_set<Point, PointHash> set;
std::unordered_set<Point, PointHash> exposed;
std::unordered_set<Point, PointHash> airPockets;

bool inBounds(Point point) {
	return point.x >= -1 && point.x < 23 &&
		point.y >= -1 && point.y < 23 &&
		point.z >= -1 && point.z < 23; 
}

void exteriorAir() {
	// basically flood fill
	std::vector<Point> outer = { Point(0, 0 ,0) }; 
	std::unordered_set<Point, PointHash> seen;
	exposed.insert(Point(0, 0 ,0));
	
	while (outer.size() > 0) {
		std::unordered_set<Point, PointHash> nextLayer;
		seen.insert(outer.begin(), outer.end());
		for (auto point : outer) {
			for (auto adjacent : point.getAdjacent()) {
				// only add if in bounds, empty space, and not already seen
				if (inBounds(adjacent) && set.find(adjacent) == set.end() && seen.find(adjacent) == seen.end()) {
					nextLayer.insert(adjacent);
					exposed.insert(adjacent);
				}
			}
		}	
		outer.clear();
		outer.insert(outer.begin(), nextLayer.begin(), nextLayer.end());
	}
}

int main() {
	std::ifstream file("resources/day18.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	std::string line;
	while (std::getline(file, line)) {
		int x = std::stoi(line);	
		line = line.substr(line.find(",") + 1);
		int y = std::stoi(line);
		line = line.substr(line.find(",") + 1);
		int z = std::stoi(line);
		set.insert(Point(x, y, z));
	}

	int surfaceArea = 0;
	int exteriorSurfaceArea = 0;

	// flood fill the area around the lava droplet
	exteriorAir();

	for (auto point : set) {
		// for every point, check how many of the adjacent spaces are occupied
		for (auto adjacent : point.getAdjacent()) {
			if (set.find(adjacent) == set.end()) {
				surfaceArea++;
				// if this is air but not part of the earlier flood fill, its a pocket
				if (exposed.find(adjacent) != exposed.end())
					exteriorSurfaceArea++;
			}
		}
	}

	std::cout << "Part 1: " << surfaceArea << "\n";
	std::cout << "Part 2: " << exteriorSurfaceArea << "\n";
}

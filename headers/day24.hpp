#ifndef DAY24
#define DAY24

#include <cstddef>
#include <functional>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

enum Direction { NORTH, SOUTH, EAST, WEST, NONE };

std::pair<int, int> getPosition(const std::string &key);

std::string getKey(int column, int row);

class Blizzard {
public:
	std::string key;
	Direction direction;

	Blizzard(const std::string &key, Direction direction) {
		this->key = key;
		this->direction = direction;
	}

	// move the blizzard one tile and account for wrapping behavior
	Blizzard move(int xBounds, int yBounds) {
		auto position = getPosition(key);
		switch (direction) {
			case NORTH:
				position.second--;
				if (position.second < 0)
					position.second = yBounds;
				break;
			case SOUTH:
				position.second++;
				if (position.second > yBounds)
					position.second = 0;
				break;
			case EAST:
				position.first++;
				if (position.first > xBounds)
					position.first = 0;
				break;
			case WEST:
				position.first--;
				if (position.first < 0)
					position.first = xBounds;
				break;
			case NONE:
				std::cerr << "Blizzards can't have a direction of none!\n";
		}
		return Blizzard(getKey(position.first, position.second), direction);
	}
};

class Node {
public:
	std::string key;
	int steps;

	Node(const std::string& key, int steps) {
		this->key = key;
		this->steps = steps;
	}

	bool operator<(const Node &other) const {
		return steps > other.steps;
	}

	bool operator==(const Node &other) const {
		if (steps != other.steps)
			return false;
		return key.compare(other.key) == 0;
	}
};

class NodeHash {
public:
	size_t operator()(const Node &node) const {
		std::stringstream hash;
		hash << node.key << "," << node.steps;
		return std::hash<std::string>()(hash.str());
	}
};
#endif

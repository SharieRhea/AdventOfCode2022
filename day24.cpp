#include "headers/day24.hpp"
#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

// hardcoded bounds :/
const int XBOUNDS = 119; // 5 for test input
const int YBOUNDS = 24; // 3 for test input
const int ARRAY_SIZE = (XBOUNDS + 1) * (YBOUNDS + 1);
const Direction directions[5] = { NORTH, SOUTH, EAST, WEST, NONE };

std::vector<Blizzard> blizzards;
std::vector<std::bitset<ARRAY_SIZE>> maps;

std::string getKey(int column, int row) {
	std::stringstream key;
	key << column << "," << row;
	return key.str();
}

std::pair<int, int> getPosition(const std::string &key) {
	int column = std::stoi(key);
	int row = std::stoi(key.substr(key.find(",") + 1));
	return std::pair<int, int>(column, row);
}

void createMap() {
	// add each blizzard's location to the bool array, then move it to the next step
	std::bitset<ARRAY_SIZE> map = { false };
	for (int i = 0; i < blizzards.size(); i++) {
		auto position = getPosition(blizzards[i].key);
		map[position.second * (XBOUNDS + 1) + position.first] = true;
		blizzards[i] = blizzards[i].move(XBOUNDS, YBOUNDS);
	}
	maps.push_back(map);
}

std::pair<int, int> move(const std::string &key, Direction direction) {
	auto position = getPosition(key);
	switch (direction) {
		case NORTH:
			position.second--;
			break;
		case SOUTH:
			position.second++;
			break;
		case EAST:
			position.first++;
			break;
		case WEST:
			position.first--;
			break;
		case NONE:
			// don't even move
			break;
	}
	return position;
}

bool isValidMove(std::pair<int, int> position, int step) {
	// special exception for the start and end points
	if (position.first == 0 && position.second == -1)
		return true;
	if (position.first == XBOUNDS && position.second == YBOUNDS + 1)
		return  true;

	// check generic bounds
	if (position.first < 0 || position.first > XBOUNDS)
		return false;
	if (position.second < 0 || position.second > YBOUNDS)
		return false;

	// check for blizzards
	std::bitset<ARRAY_SIZE> map = maps[step];
	return !map[position.second * (XBOUNDS + 1) + position.first];
}

int bfs(const std::string &start, const std::string &destination, int step) {
	std::priority_queue<Node> queue;
	Node startNode = Node(start, step);
	queue.push(startNode);
	std::unordered_set<Node, NodeHash> visited = {};
	visited.insert(queue.top());

	while (!queue.empty()) {
		Node node = queue.top();
		queue.pop();

		// see if we have reached the destination
		if (destination.compare(node.key) == 0)
			return node.steps;

		// make sure we have the map for the next step
		while (maps.empty() || maps.size() - 1 <= node.steps) {
			createMap();
		}

		// try to move in each direction
		for (auto direction : directions) {
			auto position = move(node.key, direction);
			if (isValidMove(position, node.steps + 1)) {
				auto key = getKey(position.first, position.second);
				Node next = Node(key, node.steps + 1);
				if (visited.find(next) == visited.end()) {
					queue.push(next);
					visited.insert(next);
				}
			}
		}
	}

	return -1;
}

int main() {
	std::ifstream file("resources/day24.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	// parse input
	std::string line;
	int row = -1;
	while(std::getline(file, line)) {
		int column = -1;
		for (char character : line) {
			Direction direction = NORTH;
			if (character == '.' || character == '#') {
				column++;
				continue;
			}
			switch (character) {
				case 'v':
					direction = SOUTH;
					break;
				case '>':
					direction = EAST;
					break;
				case '<':
					direction = WEST;
					break;
			}
			blizzards.push_back(Blizzard(getKey(column, row), direction));
			column++;
		}
		row++;
	}

	auto start = getKey(0, -1);
	auto destination = getKey(XBOUNDS, YBOUNDS + 1);

	// part 1 is just start to destination
	int steps = bfs(start, destination, 0);
	// forgot the snacks, go back to get em
	int backToStart = bfs(getKey(XBOUNDS, YBOUNDS + 1), start, steps);
	int andBackToDestination = bfs(start, destination, backToStart);

	std::cout << "Part 1: " << steps << "\n";
	std::cout << "Part 2: " << andBackToDestination << "\n";
}

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>

std::set<std::string> elves;
std::unordered_map<std::string, int> proposedMoves;
std::unordered_map<std::string, std::string> moves;
int direction = 0;
bool change = true;

std::string makeKey(int column, int row) {
	std::stringstream key;
	key << column << "," << row;
	return key.str();
}

std::pair<int, int> getPosition(const std::string &key) {
	int column = std::stoi(key);
	int row = std::stoi(key.substr(key.find(",") + 1));
	return std::pair(column, row);
}

bool* getAdjacent(const std::string &key) {
	auto position = getPosition(key);

	const std::pair<int, int> tiles[8] = {
		std::pair(position.first - 1, position.second - 1),	// NW
		std::pair(position.first, position.second - 1),		// N
		std::pair(position.first + 1, position.second - 1), // NE
		std::pair(position.first - 1, position.second),		// W
		std::pair(position.first + 1, position.second),		// E
		std::pair(position.first - 1, position.second + 1), // SW
		std::pair(position.first, position.second + 1),		// S
		std::pair(position.first + 1, position.second + 1), // SE
	};

	bool* occupied = static_cast<bool*>(malloc(sizeof(bool) * 8));
	for (int i = 0; i < 8; i++) {
		occupied[i] = elves.find(makeKey(tiles[i].first,  tiles[i].second)) != elves.end();
	}
	return occupied;
}

std::string checkDirection(const std::string &key, const bool* adjacent, int direction) {
	// returns the current position if direction does not have a valid move
	auto position = getPosition(key);
	switch (direction) {
		// check north (0, 1, 2)
		case 0:
			if (!adjacent[0] && !adjacent[1] && !adjacent[2])
				return makeKey(position.first, position.second - 1);
			break;
		// check south (5, 6, 7)
		case 1:
			if (!adjacent[5] && !adjacent[6] && !adjacent[7])
				return makeKey(position.first, position.second + 1);
			break;
		// check west (0, 3, 5)
		case 2:
			if (!adjacent[0] && !adjacent[3] && !adjacent[5])
				return makeKey(position.first - 1, position.second);
			break;
		// check east (2, 4, 7)
		case 3:
			if (!adjacent[2] && !adjacent[4] && !adjacent[7])
				return makeKey(position.first + 1, position.second);
			break;
	}
	return key;	
}

std::string getProposedMove(const std::string &key, int direction) {
	bool* adjacent = getAdjacent(key);

	// first, if no adjacent tiles are occupied, just return the current position
	if (std::all_of(adjacent, adjacent + 8, [](auto it) { return !it; } )) {
		free(adjacent);
		return key;
	}

	// set flag for part 2
	change = true;

	// order is N, S, W, E but wraps to the end every round, use mod
	for (int i = 0; i < 4; i++) {
		std::string proposed = checkDirection(key, adjacent, (direction + i) % 4);
		// this direction has a valid move, return it
		if (proposed.compare(key) != 0) {
			free(adjacent);
			return proposed;
		}
	}
	// can't move in any direction, stay at current position
	free(adjacent);
	return key;
}

int findEmptySpace() {
	int minX = 0;
	int minY = 0;
	int maxX = 0;
	int maxY = 0;

	for (const std::string &elf : elves) {
		auto position = getPosition(elf);
		if (position.first < minX)
			minX = position.first;
		else if (position.first > maxX)
			maxX = position.first;
		if (position.second < minY)
			minY = position.second;
		else if (position.second > maxY)
			maxY = position.second;
	}

	// number empty spaces in smallest bounding rectangle
	return (maxX - minX + 1) * (maxY - minY + 1) - elves.size();
}

void simulateRound() {
	// gather all proposed moves
	moves.clear();
	proposedMoves.clear();
	for (const std::string &elf : elves) {
		// add to map if not already present
		std::string proposed = getProposedMove(elf, direction);
		moves[elf] = proposed;
		if (proposedMoves.find(proposed) == proposedMoves.end())
			proposedMoves[proposed] = 1;
		else
			proposedMoves[proposed] = proposedMoves[proposed] + 1;
	}

	elves.clear();
	for (auto move : moves) {
		// only move if it is uncontested
		if (proposedMoves[move.second] == 1)
			elves.insert(move.second);
		else
			elves.insert(move.first);
	}

	direction++;
}

int main() {
	std::ifstream file("resources/day23.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	// parse input
	std::string line;
	int row = 0;
	while(std::getline(file, line)) {
		int column = 0;
		for (char character : line) {
			if (character == '#')
				elves.insert(makeKey(column, row));
			column++;
		}
		row++;
	}

	// part 1
	for (int round = 0; round < 10; round++) {
		simulateRound();
	}
	int emptySpaces = findEmptySpace();

	// for part 2, keep going until there is a round with no changes
	int round = 10;
	while (change) {
		change = false;
		simulateRound();
		round++;
	}

	std::cout << "Part 1: " << emptySpaces << "\n";
	std::cout << "Part 2: " << round << "\n";
}

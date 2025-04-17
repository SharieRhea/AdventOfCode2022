#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "headers/day12.hpp"

std::pair<int, int> begin;
std::pair<int, int> end;
std::vector<std::vector<char>> grid;

std::vector<std::vector<char>> parseGrid() {
	std::ifstream file("resources/day12.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	std::vector<std::vector<char>> grid;
	std::string line;
	while (std::getline(file, line)) {
		std::vector<char> row;
		for (char character : line)	{
			if (character == 'S') {
				begin = std::pair<int, int>(row.size(), grid.size());
				row.push_back('a');
			}
			else if (character == 'E') {
				end = std::pair<int, int>(row.size(), grid.size());
				row.push_back('z');
			}
			else
				row.push_back(character);	
		}
		grid.push_back(row);
	}
	return grid;
}

bool checkBounds(int x, int y) {
	return x >= 0 && x < grid[0].size() && y >= 0 && y < grid.size();
}

std::vector<std::pair<int, int>> getPoints(int x, int y) {
	std::vector<std::vector<int>> vectors = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
	std::vector<std::pair<int, int>> points;
	
	for (int i = 0; i < vectors.size(); i++) {
		if (checkBounds(x + vectors[i][0], y + vectors[i][1]))
			points.push_back(std::pair<int, int>(x + vectors[i][0], y + vectors[i][1]));
	}
	return points;
}

bool compareNodes(Node one, Node two) {
	return one.steps > two.steps;
}

int bfs(std::pair<int, int> start, std::pair<int, int> destination, int part) {
	int steps = 0;

	std::priority_queue<Node, std::vector<Node>, decltype(compareNodes)*> queue(compareNodes);
	std::set<std::string> visited;
	std::stringstream key;
	key << start.first << " " << start.second;
	visited.insert(key.str());

	queue.push(Node(start.first, start.second));

	while (!queue.empty()) {
		Node node = queue.top();
		queue.pop();
	
		bool endCondition = (part == 1) ? node.x == destination.first && node.y == destination.second : grid[node.y][node.x] == 'a';
		if (endCondition) {
			steps = node.steps;
			break;
		}

		// for each possible direction
		for (std::pair<int, int> point : getPoints(node.x, node.y)) {
			std::stringstream key;
			key << point.first << " " << point.second;
			// do not revisit any nodes
			if (visited.find(key.str()) != visited.end())
				continue;

			char nextElevation = grid[point.second][point.first];
			char currentElevation = grid[node.y][node.x];
			bool stepCondition = (part == 1) ? nextElevation - currentElevation <= 1 : nextElevation - currentElevation >= -1;
			// make sure elevation is at most one step up for part 1 and greater, same, or one step down for part 2
			if (stepCondition) {
				visited.insert(key.str());
				queue.push(Node(point.first, point.second, node.steps + 1));
			}
		}
	}

	return steps;
}

int main() {
	grid = parseGrid();

	std::cout << "Part 1: " << bfs(begin, end, 1) << "\n";
	std::cout << "Part 2: " << bfs(end, begin, 2) << "\n";
}

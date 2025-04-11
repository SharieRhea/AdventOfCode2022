#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

// this has got to be the ugliest method i have ever written
bool isVisible(int x, int y, std::vector<std::vector<int>> grid) {
	// if the tree is on an edge, it is always visible
	if (x == 0 || y == 0)
		return true;
	if (x == grid.size() - 1 || y == grid[0].size() - 1)
		return true;
	
	int height = grid[x][y];
	// check all trees above
	bool visible = true;
	for (int i = 0; i < x; i++) {
		if (grid[i][y] >= height) {
			visible = false;
			break;
		}
	}
	if (visible)
		return true;
	// check all trees below
	visible = true;
	for (int i = x + 1; i < grid.size(); i++) {
		if (grid[i][y] >= height) {
			visible = false;
			break;
		}
	}
	if (visible)
		return true;
	// check all trees left
	visible = true;
	for (int i = 0; i < y; i++) {
		if (grid[x][i] >= height) {
			visible = false;
			break;
		}
	}
	if (visible)
		return true;
	// check all trees right
	visible = true;
	for (int i = y + 1; i < grid[0].size(); i++) {
		if (grid[x][i] >= height)
			return false;
	}

	return true;
}

int getScenicScore(int x, int y, std::vector<std::vector<int>> grid) {
	int height = grid[x][y];

	int above = 0;
	for (int i = x - 1; i >= 0; i--) {
		above++;
		if (grid[i][y] >= height) {
			break;
		}
	}
	int below = 0;
	for (int i = x + 1; i < grid.size(); i++) {
		below++;
		if (grid[i][y] >= height) {
			break;
		}
	}
	int left = 0;
	for (int i = y - 1; i >= 0; i--) {
		left++;
		if (grid[x][i] >= height) {
			break;
		}
	}
	int right = 0;
	for (int i = y + 1; i < grid[0].size(); i++) {
		right++;
		if (grid[x][i] >= height)
			break;
	}
	return above * below * left * right;
}

int main() {
	std::ifstream file("resources/day08.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	// construct grid
	std::vector<std::vector<int>> grid;
	std::string line;
	while (std::getline(file, line)) {
		std::vector<int> row;

		for (int i = 0; i < line.size(); i++) {
			row.push_back(line[i] - 48);
		}
		grid.push_back(row);
	}

	int visibleTrees = 0;
	int maxScenicScore = 0;
	for (int row = 0; row < grid.size(); row++) {
		for (int column = 0; column < grid[0].size(); column++) {
			// part 1
			if (isVisible(row, column, grid))
				visibleTrees++;

			// part 2
			int scenicScore = getScenicScore(row, column, grid);
			if (scenicScore > maxScenicScore)
				maxScenicScore = scenicScore;
		}
	}

	std::cout << "Part 1: " << visibleTrees << "\n";
	std::cout << "Part 2: " << maxScenicScore << "\n";

	return 0;
}	

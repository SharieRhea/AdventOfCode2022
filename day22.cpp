#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

// NOTE: this does not work at all for the test input, since the cube faces are
// in a completely different layout
//
// test cube looks like:            my cube looks like:
//          1                                12
//        234                                3
//		    56						        54
//		                                    6

std::unordered_map<std::string, char> map;
// 0 is north, 1 is east, 2 is south, 3 is west
int direction = 1;
// for part 2, the size of each cube face
int dimensions = 50;

int currentRow = 0;
int currentColumn = dimensions;
int totalRows = dimensions * 4;
int totalColumns = dimensions * 3;

std::string makeKey(int column, int row) {
	std::stringstream key;
	key << column << "," << row;
	return key.str();
}

void move(int distance) {
	for (int i = 0; i < distance; i++) {
		// set up moving positive or negative
		int step = (direction == 0 || direction == 3) ? -1 : 1;

		// handle vertical movement
		if (direction % 2 == 0) {
			int row = currentRow + step;
			std::string next = makeKey(currentColumn, row);

			if (map.find(next) == map.end()) {
				row = (step < 0) ? totalRows : -1;
				do {
					row += step;
					next = makeKey(currentColumn, row);
				} while (map.find(next) == map.end());
			}
			if (map[next] == '#')
				break;
			currentRow = row;
		}
		// handle horizontal movement
		else {
			int column = currentColumn + step;
			std::string next = makeKey(column, currentRow);

			if (map.find(next) == map.end()) {
				column = (step < 0) ? totalColumns : -1;
				do {
					column += step;
					next = makeKey(column, currentRow);
				} while (map.find(next) == map.end());
			}
			if (map[next] == '#')
				break;
			currentColumn = column;
		}
	}
}

// from the currentRow and currentColumn, return which face of the cube we are on
int determineFace() {
	if (currentColumn >= dimensions * 2)
		return 2;
	if (currentRow >= dimensions * 3)
		return 6;
	if (currentColumn < dimensions)
		return 5;

	// must be in the middle column
	if (currentRow < dimensions)
		return 1;
	if (currentRow < dimensions * 2)
		return 3;
	return 4;
}

// this is ugly and i hate it but idk how else to do this
void cubeMove(int distance) {
	for (int i = 0; i < distance; i++) {
		// set up moving positive or negative
		int step = (direction == 0 || direction == 3) ? -1 : 1;

		// handle vertical movement
		if (direction % 2 == 0) {
			int row = currentRow + step;
			int column = currentColumn;
			int tempDirection = direction;
			std::string next = makeKey(column, row);

			if (map.find(next) == map.end()) {
				// we are wrapping to another cube face, figure out where we are first
				int face = determineFace();

				if (face == 1) {
					// moving up from 1 means going right on 6
					row = column + dimensions  * 2;
					column = 0;
					next = makeKey(column, row);
					tempDirection = 1;
				}
				else if (face == 2) {
					// moving up from 2 means going up on 6
					if (direction == 0) {
						row = dimensions * 4 - 1;
						column = column - dimensions * 2;
						next = makeKey(column, row);
						tempDirection = 0;
					}
					// moving down from 2 means going left on 2
					else {
						row = column - dimensions;
						column = dimensions * 2 - 1;;
						next = makeKey(column, row);
						tempDirection = 3;
					}
				}
				else if (face == 4) {
					// moving down from 4 means going left on 6
					row = column + dimensions * 2;
					column = dimensions - 1;
					next = makeKey(column, row);
					tempDirection = 3;
				}
				else if (face == 5) {
					// moving up from 5 means going right on 3
					row = column + dimensions;
					column = dimensions;
					next = makeKey(column, row);
					tempDirection = 1;
				}
				else if (face == 6) {
					// moving down from 6 means going down on 2
					row = 0;
					column = column + dimensions * 2;
					next = makeKey(column, row);
					tempDirection = 2;
				}
			}
			if (map[next] == '#')
				break;
			currentRow = row;
			currentColumn = column;
			direction = tempDirection;
		}
		// handle horizontal movement
		else {
			int column = currentColumn + step;
			int row = currentRow;
			std::string next = makeKey(column, row);

			if (map.find(next) == map.end()) {
				// we are wrapping to another cube face, figure out where we are first
				int face = determineFace();

				if (face == 1) {
					// moving left from 1 means going right on 5
					if (direction == 3) {
						column = 0;
						row = (dimensions * 3 - 1) - row;
						next = makeKey(column, row);
						if (map[next] == '#')
							break;
						direction = 1;
					}
				}
				else if (face == 2) {
					// moving right from 2 means going left on 4
					if (direction == 1) {
						column = dimensions * 2 - 1;
						row = (dimensions * 3 - 1) - row;
						next = makeKey(column, row);
						if (map[next] == '#')
							break;
						direction = 3;
					}
				}
				else if (face == 3) {
					// moving right from 3 means going up on 2
					if (direction == 1) {
						column = row + dimensions;
						row = dimensions - 1;
						next = makeKey(column, row);
						if (map[next] == '#')
							break;
						direction = 0;
					}
					// moving left from 3 means going down on 5
					else if (direction == 3) {
						column = row - dimensions;
						row = dimensions * 2;
						next = makeKey(column, row);
						if (map[next] == '#')
							break;
						direction = 2;

					}
				}
				else if (face == 4) {
					// moving right from 4 means going left on 2
					if (direction == 1) {
						column = dimensions * 3 - 1;
						row = (dimensions - 1) - (row - dimensions * 2);
						next = makeKey(column, row);
						if (map[next] == '#')
							break;
						direction = 3;
					}
				}
				else if (face == 5) {
					// moving left from 5 means going right on 1
					if (direction == 3) {
						column = dimensions;
						row = (dimensions - 1) - (row - dimensions * 2);
						next = makeKey(column, row);
						if (map[next] == '#')
							break;
						direction = 1;
					}
				}
				else if (face == 6) {
					// moving right from 6 means going up on 4
					if (direction == 1) {
						column = row - dimensions * 2;
						row = dimensions * 3 - 1;
						next = makeKey(column, row);
						if (map[next] == '#')
							break;
						direction = 0;
					}
					// moving left from 6 means going down on 1
					else if (direction == 3) {
						column = row - dimensions * 2;
						row = 0;
						next = makeKey(column, row);
						if (map[next] == '#')
							break;
						direction = 2;
					}
				}
			}
			if (map[next] == '#')
				break;
			currentRow = row;
			currentColumn = column;
		}
	}
}

int navigate(std::string instructions, bool part2) {
	size_t *position = static_cast<size_t*>(malloc(sizeof(size_t)));
	int distance = std::stoi(instructions, position);
	(part2) ? cubeMove(distance) : move(distance);
	instructions = instructions.substr(*position);
	while (!instructions.empty()) {
		direction = (instructions[0] == 'R') ? (direction + 1) % 4 : (direction - 1) % 4;
		if (direction < 0)
			direction += 4;
		instructions = instructions.substr(1);
		distance = std::stoi(instructions, position);
		(part2) ? cubeMove(distance) : move(distance);
		instructions = instructions.substr(*position);
	}
	free(position);

	int password = 1000 * (currentRow + 1) + 4 * (currentColumn + 1);
	if (direction == 0)
		password += 3;
	else if (direction == 2)
		password++;
	else if (direction == 3)
		password  += 2;
	return password;
}

int main() {
	std::ifstream file("resources/day22.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	std::string line;
	// parse the map
	int row = 0;
	while(std::getline(file, line)) {
		if (line.empty())
			break;

		int column = 0;
		for (char character : line) {
			if (character !=  ' ') {
				map.insert({makeKey(column, row), character});
			}
			column++;
		}
		row++;
	}

	// get the instructions line
	std::getline(file, line);

	std::cout << "Part 1: " << navigate(line, false) << "\n";
	direction = 1;
	currentRow = 0;
	currentColumn = dimensions;
	std::cout << "Part 2: " << navigate(line, true) << "\n";
}

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int NUMBER_OF_CRATES = 9;

int main() {
	std::ifstream file("resources/day05.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	// create two copies of the crates for parts 1 and 2
	std::vector<char> crates1[NUMBER_OF_CRATES];
	std::vector<char> crates2[NUMBER_OF_CRATES];

	std::string line;
	// parse the initial crate configuration
	while (getline(file, line)) {
		// end once we reach the separating blank line
		if (line.empty())
			break;

		for (int i = 0; i < NUMBER_OF_CRATES; i++) {
			// if the line is long enough and there is a letter present
			// for this crate add it
			if (i * 4 + 1 < line.length() && line[i * 4 + 1] != ' ') {
				crates1[i].insert(crates1[i].begin(), line[i * 4 + 1]);
				crates2[i].insert(crates2[i].begin(), line[i * 4 + 1]);
			}
		}
	}

	while (getline(file, line)) {
		// erase "move "
		line = line.erase(0, line.find(" ") + 1);
		int amount = std::stoi(line.substr(0, line.find(" ")));

		// erase "# from "
		line = line.erase(0, line.find(" ") + 1);
		line = line.erase(0, line.find(" ") + 1);
		int from = std::stoi(line.substr(0, line.find(" ")));

		// erase "# to "
		line = line.erase(0, line.find(" ") + 1);
		line = line.erase(0, line.find(" ") + 1);
		int to = std::stoi(line.substr(0, line.find(" ")));

		// move crates one by one from the "from" stack to the "to" stack
		for (int i = 0; i < amount; i++) {
			char crate = crates1[from - 1].back();
			crates1[from - 1].pop_back();
			crates1[to - 1].push_back(crate);
		}

		// move crates in "stacks" at a time
		for (int i = amount; i > 0; i--) {
			crates2[to - 1].push_back(crates2[from -1][crates2[from - 1].size() - i]);
		}
		for (int i = 0; i < amount; i++) {
			crates2[from - 1].pop_back();
		}
	}
	file.close();

	std::string topLevelCrates1 = "";
	for (std::vector<char> crate : crates1) {
		topLevelCrates1.push_back(crate.back());
	}
	std::string topLevelCrates2 = "";
	for (std::vector<char> crate : crates2) {
		topLevelCrates2.push_back(crate.back());
	}

	std::cout << "Part 1: " << topLevelCrates1 << "\n";
	std::cout << "Part 2: " << topLevelCrates2 << "\n";

	return 0;
}

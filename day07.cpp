#include "headers/day07.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int calculateSizes(FileSystemObject* root, std::vector<int>* sizes) {
	int size = 0;

	for (std::map<std::string, FileSystemObject*>::iterator entry = root->objects.begin(); entry != root->objects.end(); entry++) {
		// add this file's size
		size += entry->second->size;
		// only recursively add for directories further "down"
		// don't go back to the parent dir!
		if (entry->first != "..")
			size += calculateSizes(entry->second, sizes);
	}

	// add this directory's size to the list
	sizes->push_back(size);
	return size;
}

int main() {
	std::ifstream file("resources/day07.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	FileSystemObject* root = new FileSystemObject();
	FileSystemObject* current = root;

	// build the file tree
	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '$') {
			// if this is a cd command, move to that directory
			if (line[2] == 'c')
				current = current->objects[line.substr(5)];
		}
		else {
			FileSystemObject* object = new FileSystemObject();
			std::string name = line.substr(line.find(' ') + 1);
			if (line[0] == 'd') {
				// add a ".." entry that points to the parent dir
				object->objects[".."] = current;
			}
			else
				object->size = std::stoi(line.substr(0, line.find(' ')));

			// add the file or dir to the current dir's list of objects
			current->objects.insert({name, object});
		}
	}

	// store a list of the size for every directory
	std::vector<int> directorySizes;
	int rootSize = calculateSizes(root, &directorySizes);

	int part1Size = 0;
	// start with maximum space on disk
	int part2Size = 70000000;

	for (int size : directorySizes) {
		if (size <= 100000)
			part1Size += size;
		// if we are left with > 30000000 free space and this dir is smaller than previous, update it
		if (70000000 - rootSize + size > 30000000 && size < part2Size)
			part2Size = size;
	}

	std::cout << "Part 1: " << part1Size << "\n";
	std::cout << "Part 2: " << part2Size << "\n";

	return 0;
}

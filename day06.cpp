#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

int main() {
	std::ifstream file("resources/day06.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	std::string line;
	getline(file, line);

	int index = 0;
	int packetIndex = -1;
	int messageIndex = -1;

	while (packetIndex == -1 || messageIndex == -1) {
		// a packet must have 4 distinct continuous characters
		if (packetIndex == -1) {
			std::unordered_set<char> packet;
			for (int i = index; i < index + 4; i++) {
				packet.insert(line[i]);
			} 
			if (packet.size() > 3)
				packetIndex = index + 4; 
		}
		// a message must have 14 distinct continuous characters
		std::unordered_set<char> message;
		for (int i = index; i < index + 14; i++) {
			message.insert(line[i]);
		} 
		if (message.size() > 13)
			messageIndex = index + 14; 

		index++;
	}
	file.close();

	std::cout << "Part 1: " << packetIndex << "\n";
	std::cout << "Part 2: " << messageIndex << "\n";

	return 0;
}

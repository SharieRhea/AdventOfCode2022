#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "headers/day13.hpp"

int index = 0;
std::string line;

int dividerPacket1 = -1;
int dividerPacket2 = -1;

void parseList(ListItem* current) {
	while (index < line.length()) {
		if (line[index] == '[') {
			// starting a new list
			ListItem* next = new ListItem(-1);
			current->list.push_back(next);
			index++;
			parseList(next);
		}
		else if (line[index] == ']') {
			// ending this list
			index++;
			return;
		}
		else if (line[index] == ',') {
			index++;
		}
		else {
			int commaIndex = line.find(',', index);
			int bracketIndex = line.find(']', index);

			int number;
			if (commaIndex != -1 && commaIndex < bracketIndex) {
				// there is a number followed by a ,
				number = std::stoi(line.substr(index, commaIndex - index));
				index = commaIndex + 1;
				ListItem* item = new ListItem(number);
				current->list.push_back(item);
			}
			else {
				// there is number followed by a ]
				number = std::stoi(line.substr(index, bracketIndex - index));
				index = bracketIndex;
				ListItem* item = new ListItem(number);
				current->list.push_back(item);
			}
		}
	}	
}

// returns 0 for out of order, 1 for in order, and 2 for continue checking
int compareLists(ListItem* left, ListItem* right) {
	int itemIndex = 0;
	while (true) {
		// same length, keep checking
		if (itemIndex >= left->list.size() && itemIndex >= right->list.size())
			return 2;
		// left list ran out first, in order
		else if (itemIndex >= left->list.size())
			return 1;
		// right list ran out first, not in order
		else if (itemIndex >= right->list.size())
			return 0;

		int leftNumber = left->list[itemIndex]->number;
		int rightNumber = right->list[itemIndex]->number;

		// both sides are lists
		if (leftNumber == -1 && rightNumber == -1) {
			int result = compareLists(left->list[itemIndex], right->list[itemIndex]);
			// if lists were the same, keep checking, otherwise return result
			if (result != 2)
				return result;
		}
		// both sides are numbers
		else if (leftNumber != -1 && rightNumber != -1) {
			if (leftNumber > rightNumber)
				return 0;
			else if (leftNumber < rightNumber)
				return 1;
		}
		else {
			ListItem *leftList = left->list[itemIndex];
			ListItem *rightList = right->list[itemIndex];

			// only left side is a list
			if (leftNumber == -1) {
				ListItem* newRight = new ListItem(-1);
				ListItem* entry = new ListItem(right->list[itemIndex]->number);
				newRight->list.push_back(entry);
				rightList = newRight;
			}
			// only right side is a list
			else if (rightNumber == -1) {
				ListItem* newLeft = new ListItem(-1);
				ListItem* entry = new ListItem(left->list[itemIndex]->number);
				newLeft->list.push_back(entry);
				leftList = newLeft;
			}
			int result = compareLists(leftList, rightList);
			// if lists were the same, keep checking, otherwise return result
			if (result != 2)
				return result;
		}
		itemIndex++;
	}
}

ListItem* createDivider(int number) {
	ListItem *divider = new ListItem(-1);
	ListItem* subList1 = new ListItem(-1);
	ListItem* subList2 = new ListItem(-1);
	subList2->list.push_back(new ListItem(number));
	subList1->list.push_back(subList2);
	divider->list.push_back(subList1);
	return divider;
}

void findDividers(std::vector<ListItem*> packets, ListItem *divider1, ListItem *divider2) {
	int i = 0;
	while (dividerPacket1 == -1 || dividerPacket2 == -1) {
		if (packets[i] == divider1)
			dividerPacket1 = i + 1;
		else if (packets[i] == divider2)
			dividerPacket2 = i + 1;
		i++;
	}
}

int main() {
	std::ifstream file("resources/day13.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	std::vector<ListItem*> packets;
	while (std::getline(file, line)) {
		if (line.empty())
			continue;
		ListItem *head = new ListItem(-1);
		parseList(head);
		packets.push_back(head);
		index = 0;
	}

	int sum = 0;
	int packetIndex = 1;
	for (int i = 0; i < packets.size(); i += 2) {
		int value = compareLists(packets[i], packets[i + 1]);
		if (value == 1) {
			sum += packetIndex;
		}
		packetIndex++;
	}

	// part 2, add the divider packets in
	ListItem *divider1 = createDivider(2);
	ListItem *divider2 = createDivider(6);
	packets.push_back(divider1);
	packets.push_back(divider2);

	// sort then find the dividers
	std::sort(packets.begin(), packets.end(), compareLists);
	findDividers(packets, divider1, divider2);

	std::cout << "Part 1: " << sum << "\n";
	std::cout << "Part 2: " << dividerPacket1 * dividerPacket2 << "\n";
}

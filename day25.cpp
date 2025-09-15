#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int convertDigit(char digit) {
	switch (digit) {
		case '2':
			return 2;
		case '1':
			return 1;
		case '0':
			return 0;
		case '-':
			return -1;
		case '=':
			return -2;
		default:
			std::cerr << "Invalid SNAFU digit!\n";
			exit(1);
	}
}

long long processSNAFU(std::string number) {
	// reverse the string to make it easier
	std::reverse(number.begin(), number.end());

	long long sum = 0;
	for (int i = 0; i < number.size(); i++) {
		sum += convertDigit(number[i]) * std::pow(5, i);
	}
	return sum;
}

int main() {
	std::ifstream file("resources/day25.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	std::string line;
	long long sum = 0;
	while (getline(file, line)) {
		sum += processSNAFU(line);
	}
	file.close();

	// convert back to SNAFU
	std::stringstream result;
	long long remainder = sum;
	const char digits[5] = { '=', '-', '0', '1', '2' };
	while (true) {
		result << digits[(remainder + 2) % 5];
		remainder = (remainder + 2) / 5;
		if (remainder == 0)
			break;
	}

	// reverse the string
	std::string snafu = result.str();
	std::reverse(snafu.begin(), snafu.end());

	std::cout << "Part 1: " << snafu << "\n";

	return 0;
}

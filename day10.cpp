#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

int registerValue = 1;
int clockCycle = 0;
int signalStrength = 0;
std::ofstream outfile("day10out.txt");

void printToCRT() {
	// if the 3-wide sprite overlaps the current horizontal position
	// light it up
	if (std::abs(registerValue - clockCycle % 40) < 2)
		outfile << '#';
	else
		outfile << '.';
}

void checkClock() {
	printToCRT();	
	clockCycle++;
	// add during cycles 20, 60, 100, ...
	if ((clockCycle - 20) % 40 == 0) 
		signalStrength += registerValue * clockCycle;
	// start a new CRT row
	if (clockCycle % 40 == 0)
		outfile << "\n";
}

int main() {	
	std::ifstream file("resources/day10.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		return 1;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (clockCycle > 240)
			break;

		// noop instruction
		if (line[0] == 'n') {
			checkClock();
			continue;	
		}

		// add instruction
		checkClock();
		checkClock();
		registerValue += std::stoi(line.substr(5));
	}
	outfile.close();

	std::cout << "Part 1: " << signalStrength << "\n";
	std::cout << "Part 2: see day10out.txt!\n";
}

#ifndef DAY19
#define DAY19

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

class Blueprint {
public:
	int oreCost;
	int clayCost;
	std::pair<int, int> obsidianCost;
	std::pair<int, int> geodeCost;

	explicit Blueprint(std::string line) {
		int costs[6];
		for (int i = 0; i < 2; i++) {
			line = line.substr(line.find("costs") + 5);
			costs[i] = std::stoi(line);
		}
		for (int i = 2; i < 6; i += 2) {
			line = line.substr(line.find("costs") + 5);
			costs[i] = std::stoi(line);
			line = line.substr(line.find("and") + 3);
			costs[i + 1] = std::stoi(line);
		}
		oreCost = costs[0];
		clayCost = costs[1];
		obsidianCost = std::pair<int, int>(costs[2], costs[3]);
		geodeCost = std::pair<int, int>(costs[4], costs[5]);
	}
};

class Factory {
public:
	int oreRobots = 1;
	int clayRobots = 0;
	int obsidianRobots = 0;
	int geodeRobots = 0;

	int ore = 0;
	int clay = 0;
	int obsidian = 0;
	int geodes = 0;

	int time = 0;

	// default constructor
	Factory() {}

	// copy constructor
	explicit Factory(Factory const *factory) {
		oreRobots = factory->oreRobots;
		clayRobots = factory->clayRobots;
		obsidianRobots = factory->obsidianRobots;
		geodeRobots = factory->geodeRobots;

		ore = factory->ore;
		clay = factory->clay;
		obsidian = factory->obsidian;
		geodes = factory->geodes;

		time = factory->time;
	}

	bool operator==(const Factory& other) const {
		return this->ore == other.ore && this->clay == other.clay && this->obsidian == other.obsidian && this->geodes == other.geodes &&
			this->oreRobots == other.oreRobots && this->clayRobots == other.clayRobots && this->obsidianRobots == other.obsidianRobots &&
			this->geodeRobots == other.geodeRobots;
	}

	std::string hash() const {
		std::stringstream value;
		value <<  ore << ", " << clay << ", " << obsidian << ", " << geodes << ";";
		value << oreRobots << ", " << clayRobots << ", " << obsidianRobots << ", " << geodeRobots << ";";
		value << time;
		return value.str();
	}
};

class CompareFactory {
public:
    bool operator()(const Factory &left, const Factory &right) {
		return left.geodes < right.geodes;
    }
};

#endif

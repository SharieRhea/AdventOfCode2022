#include "headers/day19.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

std::set<std::string> seen;

// collect all resources from the current robots
void collectResources(Factory* factory, int minutes) {
	factory->ore += minutes * factory->oreRobots;
	factory->clay += minutes * factory->clayRobots;
	factory->obsidian += minutes * factory->obsidianRobots;
	factory->geodes += minutes * factory->geodeRobots;
}

// determine how many geodes this state will end with (basically add remaining created geodes from
// the time of last robot creation to the end)
int getFinalGeodes(Factory const* factory, int time) {
	return factory->geodes + factory->geodeRobots * (time - factory->time);
}

// determine how many geodes this state could HYPOTHETICALLY create if we built a new geode
// robot every minute for each remaining minute
int getMaxGeodes(Factory const* factory, int time) {
	int remainingTime = time - factory->time;
	return factory->geodes + factory->geodeRobots * remainingTime + ((remainingTime * (remainingTime - 1)) / 2);
}

// determine if we should keep this factory based on its current state and the current time
bool prune(Factory const *factory, Blueprint const *blueprint, int maxGeodes, int time) {
	// prune duplicate states
	if (seen.find(factory->hash()) != seen.end())
		 return true;
	// if the max number of geodes this state could produce is less than the max we've found
	// so far, prune it
	if (maxGeodes > getMaxGeodes(factory, time) )
		return true;
	// check that we do not have more robots than could ever be needed to build any given type of robot
	if (factory->oreRobots > std::max({ blueprint->oreCost, blueprint->clayCost, blueprint->obsidianCost.first, blueprint->geodeCost.first }) ||
		factory->clayRobots > blueprint->obsidianCost.second || 
		factory->obsidianRobots > blueprint->geodeCost.second)
		return true;

	return false;
}

int ceil(int numerator, int denominator) {
	return (numerator + denominator - 1) / denominator;
}

// create a new state for building each next possible robot within the remaining time
Factory createOreRobot(Factory const *factory, Blueprint const *blueprint) {
	// it always costs at least 1 minute to build the robot
	int minutes = 1;
	// add time needed to collect resources
	int oreToCollect = blueprint->oreCost - factory->ore;
	if (oreToCollect > 0)
		minutes += ceil(oreToCollect, factory->oreRobots);

	Factory newFactory = Factory(factory);
	collectResources(&newFactory, minutes);
	newFactory.oreRobots++;
	newFactory.ore -= blueprint->oreCost;
	newFactory.time += minutes;
	return newFactory;
}

Factory createClayRobot(Factory const *factory, Blueprint const *blueprint) {
	// it always costs at least 1 minute to build the robot
	int minutes = 1;
	// add time needed to collect resources
	int oreToCollect = blueprint->clayCost - factory->ore;
	if (oreToCollect > 0)
		minutes += ceil(oreToCollect, factory->oreRobots);

	Factory newFactory = Factory(factory);
	collectResources(&newFactory, minutes);
	newFactory.clayRobots++;
	newFactory.ore -= blueprint->clayCost;
	newFactory.time += minutes;
	return newFactory;
}

Factory createObsidianRobot(Factory const *factory, Blueprint const *blueprint) {
	// it always costs at least 1 minute to build the robot
	int minutes = 1;
	// add time needed to collect resources, whichever is longer ore or clay
	int oreToCollect = blueprint->obsidianCost.first - factory->ore;
	int clayToCollect = blueprint->obsidianCost.second - factory->clay;
	if (oreToCollect > 0 || clayToCollect > 0)
		minutes += std::max(ceil(oreToCollect, factory->oreRobots), ceil(clayToCollect, factory->clayRobots));

	Factory newFactory = Factory(factory);
	collectResources(&newFactory, minutes);
	newFactory.obsidianRobots++;
	newFactory.ore -= blueprint->obsidianCost.first;
	newFactory.clay -= blueprint->obsidianCost.second;
	newFactory.time += minutes;
	return newFactory;
}

Factory createGeodeRobot(Factory const *factory, Blueprint const *blueprint) {
	// it always costs at least 1 minute to build the robot
	int minutes = 1;
	// add time needed to collect resources, whichever is longer ore or obsidian
	int oreToCollect = blueprint->geodeCost.first - factory->ore;
	int obsidianToCollect = blueprint->geodeCost.second - factory->obsidian;
	if (oreToCollect > 0 || obsidianToCollect > 0)
		minutes += std::max(ceil(oreToCollect, factory->oreRobots), ceil(obsidianToCollect, factory->obsidianRobots));

	Factory newFactory = Factory(factory);
	collectResources(&newFactory, minutes);
	newFactory.geodeRobots++;
	newFactory.ore -= blueprint->geodeCost.first;
	newFactory.obsidian -= blueprint->geodeCost.second;
	newFactory.time += minutes;
	return newFactory;
}

int blueprintMax(const Blueprint *blueprint, int time) {
	int maxGeodes = 0;
	std::priority_queue<Factory, std::vector<Factory>, CompareFactory> queue;
	queue.push(Factory());

	while(!queue.empty()) {
		Factory const factory = queue.top();
		seen.insert(factory.hash());
		queue.pop();

		// 5 possible options to branch from this state
		// see what would happen if we didn't build any more robots at all
		int geodes = getFinalGeodes(&factory, time);
		if (geodes > maxGeodes)
			maxGeodes = geodes;

		// try to create each of the 4 robot types
		Factory ore = createOreRobot(&factory, blueprint);
		if (ore.time < time && !prune(&ore, blueprint, maxGeodes, time))
			queue.push(ore);
		Factory clay = createClayRobot(&factory, blueprint);
		if (clay.time < time && !prune(&clay, blueprint, maxGeodes, time))
			queue.push(clay);
		// make sure we have at least one clay robot otherwise an obsidianRobot is not possible
		if (factory.clayRobots != 0) {
			Factory obsidian = createObsidianRobot(&factory, blueprint);
			if (obsidian.time < time && !prune(&obsidian, blueprint, maxGeodes, time))
				queue.push(obsidian);
		}
		// make sure we have at least one obsidian robot otherwise a geodeRobot is not possible
		if (factory.obsidianRobots != 0) {
			Factory geode = createGeodeRobot(&factory, blueprint);
			if (geode.time < time && !prune(&geode, blueprint, maxGeodes, time))
				queue.push(geode);
		}
	}
	seen.clear();
	return maxGeodes;
}

int main() {
	std::ifstream file("resources/day19.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}

	std::vector<Blueprint*> blueprints;
	std::string line;
	while (std::getline(file, line)) {
		blueprints.push_back(new Blueprint(line));
	}

	// part 1
	std::vector<int> maxes;
	for (auto const blueprint : blueprints) {
		maxes.push_back(blueprintMax(blueprint, 24));
	}
	int quality = 0;
	for (int i = 0; i < maxes.size(); i++) {
		quality += maxes[i] * (i + 1);
	} 
	std::cout << "Part 1: " << quality << "\n";

	// part 2
	int multiplied = 1;
	for (int i = 0; i < 3; i++) {
		multiplied *= blueprintMax(blueprints[i], 32);
	}
	std::cout << "Part 2: " << multiplied << "\n";
}

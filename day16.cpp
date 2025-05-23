#include "headers/day16.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

std::map<std::string, int> valves;
std::map<std::string, std::vector<std::string>> tunnels;
// format is SS->DD, steps to take
std::map<std::string, int> paths;

std::vector<std::string> parseList(std::string input) {
	std::vector<std::string> returnValue;
	
	// set index to the beginning of the first destination valve
	int index = input.length() - 2;
	if (input.find(",") != -1)
		index = input.find(",") - 2;
	input = input.substr(index);

	while (input.length() > 2) {
		returnValue.push_back(input.substr(0, 2));
		// remove the "XX, "
		input = input.substr(4);
	}
	// don't forget the last value
	returnValue.push_back(input.substr(0, 2));
	return returnValue;
}

void buildGraph(std::string start, std::string current, int step) {
	// use bfs to search paths
	for (auto destination : tunnels[current]) {
		std::stringstream key;
		key << start << "->" << destination;
		
		// if the path is newly discovered or shorter than the existing one, add to map and search further
		if (paths.find(key.str()) == paths.end() || paths[key.str()] >= step + 1) {
			paths[key.str()] = step + 1;
			if (start.compare(destination) != 0)
				buildGraph(start, destination, step + 1);
		}
	}
}

// return a (destination, step) pair within the provided step limit, with nonzero flow, and not opened
std::vector<std::pair<std::string, int>> getReachableValves(std::string start, int steps, std::set<std::string> opened) {
	std::vector<std::pair<std::string, int>> returnValue;
	for (auto valve : valves) {
		// ignore if the start and destination are the same or if there is no flow or if valve is already opened 
		if (start.compare(valve.first) == 0 || 
			valve.second == 0 || 
			opened.find(valve.first) != opened.end()
		)
			continue;

		std::stringstream key;
		key << start << "->" << valve.first;
		// make sure there is enough time to reach the destination
		if (paths[key.str()] <= steps)
			returnValue.push_back(std::pair<std::string, int>(valve.first, paths[key.str()]));
	}
	return returnValue;
}

bool compare(State one, State two) {
	return one.flow < two.flow;
}

std::map<std::set<std::string>, int> openValves(int seconds) {
	// bfs
	// keep track of visited states in the format time,pressure,visited
	std::set<std::string> visited;
	// keep track of the max relief for a given set of opened valves
	std::map<std::set<std::string>, int> relief;
	std::priority_queue<State, std::vector<State>, decltype(compare)*> queue(compare);
	queue.push(State("AA", seconds, 0, 0, std::set<std::string>()));
	int maxPressure = 0;

	while (!queue.empty()) {
		State state = queue.top();
		queue.pop();

		// determine all reachable valves with nonzero flow (within time limit)
		auto reachable = getReachableValves(state.currentValve, state.time - 1, state.openedValves);
		for (auto destination : reachable) {
			std::stringstream visitedKey;
			visitedKey << state.time - destination.second - 1 << "," << state.pressure << ",";
			std::stringstream reliefKey;

			std::set<std::string> opened;
			// copy the previous state's open valves
			for (auto open : state.openedValves) {
				visitedKey << open << ",";
				reliefKey << open << ",";
				opened.insert(open);
			}
			visitedKey << destination.first;
			// insert the valve we are opening during this minute
			opened.insert(destination.first);

			// previous pressure + flow rate * steps it takes to get to next valve (and the minute to open the next one) + newly opened valve
			int pressure = state.pressure + state.flow * (destination.second + 1) + valves[destination.first];

			if (visited.find(visitedKey.str()) == visited.end()) {
				visited.insert(visitedKey.str());
				queue.push(State(destination.first, state.time - destination.second - 1, pressure, state.flow + valves[destination.first], opened));
			}

			if (relief.find(opened) == relief.end() || pressure > relief[opened])
				relief[opened] = pressure;
		}

		// calculate pressure for the remaining time if you chose to not open any more valves
		int pressure = state.pressure + state.flow * state.time;

		if (relief.find(state.openedValves) == relief.end() || pressure > relief[state.openedValves])
			relief[state.openedValves] = pressure;
	}

	return relief;	
}

int main() {
	std::ifstream file("resources/day16.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open input file!";
		exit(1);
	}
	std::string current;

	std::string line;
	while(std::getline(file, line)) {
		// get the name of this valve
		std::string key = line.substr(6, 2);

		// if this is the first key, this is the starting point
		if (current.compare("") == 0) 
			current = key;

		int flow = std::stoi(line.substr(23));
		valves.insert({ key, flow });
		// find the first comma OR end of line and subtract 2 to find the start of the list
		int index = line.find(",");
		if (index == -1)
			index = line.length() - 2;
		std::vector<std::string> destinations = parseList(line.substr(index - 2));
		tunnels.insert({ key, destinations });
	}

	// fully explore graph
	for (auto valve : valves) {
		buildGraph(valve.first, valve.first, 0);
	}

	// for part 1 simply find the max pressure released
	auto part1 = openValves(29);
	int pressureAlone = 0;
	for (auto value : part1) {
		if (value.second > pressureAlone)
			pressureAlone = value.second;
	}

	// for part 2 look for two sets that are completely disjoint,
	// the pressure is their sum
	auto part2 = openValves(25);
	int pressureTogether = 0;

	// this is ugly, could be so much shorter
	for (auto set1 : part2) {
		for (auto set2 : part2) {
			bool disjoint = true;
			for (auto key : set1.first) {
				if (set2.first.find(key) != set2.first.end()) {
					disjoint = false;
					break;
				}
			}
			if (!disjoint)
				continue;
			for (auto key : set2.first) {
				if (set1.first.find(key) != set1.first.end()) {
					disjoint = false;
					break;
				}
			}
			if (disjoint && set1.second + set2.second > pressureTogether)
				pressureTogether = set1.second + set2.second;
		}
	}

	std::cout << "Part 1: " << pressureAlone << "\n";
	std::cout << "Part 2: " << pressureTogether << "\n";
}

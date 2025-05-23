#ifndef DAY16
#define DAY16

#include <set>
#include <string>
class State {
public:
	std::string currentValve;
	int time;
	std::set<std::string> openedValves;
	int pressure = 0;
	int flow = 0;
	
	State(std::string currentValve, int time, int pressure, int flow, std::set<std::string> openedValves) {
		this->currentValve = currentValve;
		this->time = time;
		this->pressure = pressure;
		this->flow = flow;
		this->openedValves = openedValves;
	}
};

#endif

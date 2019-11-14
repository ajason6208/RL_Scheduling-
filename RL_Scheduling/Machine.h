#pragma once

#ifndef MACHINE_H_
#define MACHINE_H_

#include <vector>

using namespace std;

class Machine {
public:
	Machine(const int& machine_id, const double& process_time); // default constructor
	~Machine();

	// get private variable
	int& get_machine_id();
	double& get_process_time();

private:
	int machine_id;
	double process_time;
};


#endif
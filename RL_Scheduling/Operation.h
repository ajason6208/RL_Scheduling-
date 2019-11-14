#pragma once

#ifndef OPERATION_H_
#define OPERATION_H_

#include <vector>

#include "Machine.h"

using namespace std;

class Operation {


public:
	Operation(const int& ope_id, const vector<Machine>& available_machines); // default constructor
	~Operation();

	// set private variable
	void set_available_machines(const Machine& machine);

	// get private variable
	vector<Machine>& get_available_machines();

private:
	int ope_id;
	vector<Machine> available_machines;
};


#endif
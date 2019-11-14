#pragma once

#ifndef JOB_H_
#define JOB_H_

#include <vector>

#include "Operation.h"

using namespace std;

class Job {


public:
	Job(); // default constructor
	~Job();

	// set private variable
	void set_job_id(const int& job_id);
	void set_operations(const vector<Operation>& operations);

	// get private variable
	int get_job_id();
	vector<Operation>& get_operations();

private:
	int job_id;
	vector<Operation> operations;
};


#endif
#pragma once

#ifndef SCHEDULING_H_
#define SCHEDULING_H_

#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

#include "Job.h"
#include "config.h"
#include "schedule.h"

using namespace std;

class Scheduling {


public:
	// Read or-lib
	Scheduling();
	Scheduling(const string& file_name, const int& index); // index �u�O���F���Ӥ�KŪ���P���, index = 1 �� or-lib
	~Scheduling();

	// �����C�� Job ���ӳ���T
	vector<Job> demands;

	// �����Կ��I
	map<JOB_ID, Candidate> job_cadidate;

	// ���� machine & job release time
	map<JOB_ID, RELEASE_TIME> job_release_time;
	map<MACHINE_ID, RELEASE_TIME> machine_release_time;

	// �����`�@���h�֭� operations
	int sum_of_operations;

	// Get Function
	int Get_Sum_Ops();

	// set private variable
	void set_demands(const Job& job);
	void set_job_cadidate(const int& job_id, const int& ope_id);

	// get private variable
	int get_sum_of_operations();

	// Initialize Variable
	void Initialize_Job_Release_Time();
	void Initialize_Machine_Release_Time();
	void Initialize_Job_Cadidate();
private:
	void Initialize_Sum_Of_Ops();
};


#endif
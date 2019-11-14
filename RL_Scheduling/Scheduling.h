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
	Scheduling(const string& file_name, const int& index); // index 只是為了未來方便讀不同資料, index = 1 為 or-lib
	~Scheduling();

	// 紀錄每個 Job 的細部資訊
	vector<Job> demands;

	// 紀錄候選點
	map<JOB_ID, Candidate> job_cadidate;

	// 紀錄 machine & job release time
	map<JOB_ID, RELEASE_TIME> job_release_time;
	map<MACHINE_ID, RELEASE_TIME> machine_release_time;

	// 紀錄總共有多少個 operations
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
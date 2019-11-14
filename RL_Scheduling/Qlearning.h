#pragma once

#ifndef QLEARNING_H_
#define QLEARNING_H_

#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
#include <iostream>
#include <random>
#include <tuple>
#include <iterator>

#include "Job.h"
#include "config.h"
#include "schedule.h"
#include "Scheduling.h"

using namespace std;

class QLearning {
public:
	// Read or-lib
	QLearning(Scheduling& scheduling);
	~QLearning();

	Scheduling scheduling;

	// set private variable
	void set_q_table_row(const vector<double>& q_row);
	void set_q_table(vector<vector<double>>& i_q_table);
	void set_scheduled_tasks(const ScheduledTask& scheduled_task);

	// get private variable
	vector<vector<double>> get_q_table();
	vector<vector<double>> get_best_q_table();

	// Q learning
	void start();
	void record();
	void reset();
	Result test();
	Candidate GetJobCandidate();
	void UpdateReleaseTime(const Candidate& candidate);
	Reward CalReward(vector<ScheduledTask>& scheduled_tasks, const Candidate& candidate);
	void UpdateJobCandidate(const Candidate& candidate);
	void UpdateQTable(const Candidate& candidate, const Reward& reward);
	double GetNextStatesMaxQValue(const Candidate& candidate);

	// Q learning parameter
	double episode = 100000;
	double greedy = 0.1; // greedy
	double gamma = 0.9;
	double alpha = 0.9; 
	double denoise = 0.5;
	double qBetter = 0.5;

	// 記錄每一 episode 的結果
	vector<double> makespans;
	vector<vector<ScheduledTask>> all_scheduled_tasks;
	vector<vector<double>> best_q_tables;
	double best_makespan = 9999.0;

private:
	vector<vector<double>> q_table;
	vector<ScheduledTask> scheduled_tasks;

	// Initialize Variable
	void Initialize_Q_Table(vector<Job> demands);
};


#endif
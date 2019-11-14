#pragma once

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <vector>

using namespace std;

struct Candidate {
	//Candidate(int j, int op, int ros) : job_id(j), ope_id(op), remain_ops(ros){}
	int job_id;
	int ope_id;
	int remain_ops;
};

struct ScheduledTask {
	int job_id;
	int ope_id;
	vector<int> machine_id;
	vector<double> process_time;
	double start_time;
	double end_time;
};

struct Reward {
	double makespan; // max(所有job目前工序的完工時間) → 希望別做太久
	double space_bw_last_job;// 相同 job 下，目前排的這工序和前一道工序的間隔(job_view)
	double space_bw_last_machine;// 目前排的這工序所使用的 machine 和該台 machine 上次被使用完後的間隔
	double process_time; // 選到的候選人的加工時間
	double machine_release_time; // 選到的候選人排下去之後的機台 release time
	double job_release_time; // 選到的候選人排下去之後的job release time
};

struct Result {
	double makespan;
};

#endif

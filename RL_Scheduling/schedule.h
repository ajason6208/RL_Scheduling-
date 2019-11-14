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
	double makespan; // max(�Ҧ�job�ثe�u�Ǫ����u�ɶ�) �� �Ʊ�O���Ӥ[
	double space_bw_last_job;// �ۦP job �U�A�ثe�ƪ��o�u�ǩM�e�@�D�u�Ǫ����j(job_view)
	double space_bw_last_machine;// �ثe�ƪ��o�u�ǩҨϥΪ� machine �M�ӥx machine �W���Q�ϥΧ��᪺���j
	double process_time; // ��쪺�Կ�H���[�u�ɶ�
	double machine_release_time; // ��쪺�Կ�H�ƤU�h���᪺���x release time
	double job_release_time; // ��쪺�Կ�H�ƤU�h���᪺job release time
};

struct Result {
	double makespan;
};

#endif

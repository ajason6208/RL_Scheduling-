#include "QLearning.h"
#include "util.h"


QLearning::QLearning(Scheduling& scheduling) {
	// Initialize Variable
	Initialize_Q_Table(scheduling.demands);
	this->scheduling = scheduling;
}

QLearning::~QLearning() {

}

// set private variable
void QLearning::set_q_table_row(const vector<double>& q_row) {
	q_table.push_back(q_row);
}

void QLearning::set_q_table(vector<vector<double>>& i_q_table) {
	q_table = i_q_table;
}

void QLearning::set_scheduled_tasks(const ScheduledTask& scheduled_task){
	this->scheduled_tasks.push_back(scheduled_task);
}

// get private variabe
vector<vector<double>> QLearning::get_q_table() {
	return this->q_table;
}
vector<vector<double>> QLearning::get_best_q_table() {
	return this->best_q_tables;
}

// Initialize Variable
void QLearning::Initialize_Q_Table(vector<Job> demands) {
	for (auto it = demands.begin(); it != demands.end(); ++it) {
		vector<double> temp;
		for (auto it2 = it->get_operations().begin(); it2 != it->get_operations().end(); ++it2) {
			temp.push_back(0.0);
		}
		set_q_table_row(temp);
	}
}

// Q Learning
Candidate QLearning::GetJobCandidate() {
	// Random engine
	random_device rd;
	default_random_engine gen = default_random_engine(rd());
	uniform_real_distribution<> dis_0_1(0, 1);
	uniform_int_distribution<int> dis(0, static_cast<int>(scheduling.job_cadidate.size() - 1));
	double epislon = dis_0_1(gen);
	
	if (epislon > greedy) {
		// Initialize variable
		double max_q_value = 0.0;
		size_t index = 0;
		auto it_ini = scheduling.job_cadidate.begin();
		Candidate candidate{ it_ini->second.job_id, it_ini->second.ope_id, it_ini->second.remain_ops};
		max_q_value = q_table[it_ini->second.job_id][it_ini->second.ope_id];
		// Get all candidate
		for (auto it = scheduling.job_cadidate.begin(); it != scheduling.job_cadidate.end(); ++it) {
			// Find max value in Q table => Q(St, at), 其中 Q 的策略是 argmax Q(s,a) : 在狀態 s 時採取能獲得最大回報的動作		
			if (q_table[it->second.job_id][it->second.ope_id] > max_q_value) {
				candidate.job_id = it->second.job_id; 
				candidate.ope_id = it->second.ope_id; 
				candidate.remain_ops = it->second.remain_ops;
				max_q_value = q_table[it->second.job_id][it->second.ope_id];
			}
		}
		// 如果 max_q_value = 0.0 表示全部候選人都 q value 都相同 => 隨機選一個
		if (max_q_value == 0.0) {
			try {
				int index = dis(gen); //產生亂數
				auto it = scheduling.job_cadidate.begin();
				advance(it, index);//一次跳 index 距離
				candidate.job_id = it->second.job_id; candidate.ope_id = it->second.ope_id; candidate.remain_ops = it->second.remain_ops;
			}
			catch(exception e){
				cout << e.what() << endl;
			}
		}
		return candidate;
	}
	else {	// 增加探索可能性，選 50% 以下的
		// Normalize Candidate Q to 0~1
		vector<double> tmp;
		vector<int> inx_more_than_zero;
		for (auto it = scheduling.job_cadidate.begin(); it != scheduling.job_cadidate.end(); ++it) {
			tmp.push_back(q_table[it->second.job_id][it->second.ope_id]);
		}
		double min = *min_element(tmp.begin(), tmp.end());
		double max = *max_element(tmp.begin(), tmp.end());
		if ((max - min) != 0) {
			for (auto it = tmp.begin(); it != tmp.end(); ++it) {
				*it = (*it - min) / (max - min);
				if (*it <= qBetter) {
					inx_more_than_zero.push_back(static_cast<int>(it - tmp.begin()));
				}
			}
		}
		// Choose Q >= qBetter Candidate
		if (!inx_more_than_zero.empty()) {
			random_shuffle(inx_more_than_zero.begin(), inx_more_than_zero.end());
			auto it = scheduling.job_cadidate.begin();
			advance(it, inx_more_than_zero[0]);//一次跳 index 距離
			return Candidate{ it->second.job_id, it->second.ope_id, it->second.remain_ops };
		}
		else {
			int index = dis(gen); //產生亂數
			auto it = scheduling.job_cadidate.begin();
			advance(it, index);//一次跳 index 距離
			Candidate candidate{ it->second.job_id, it->second.ope_id, it->second.remain_ops };
			return candidate;
		}
		
		//if (dis_0_1(gen) > 0.7) {
		//	// Random one key in job cadidate
		//	int index = dis(gen); //產生亂數
		//	auto it = scheduling.job_cadidate.begin();
		//	advance(it, index);//一次跳 index 距離
		//	Candidate candidate{ it->second.job_id, it->second.ope_id, it->second.remain_ops };
		//	return candidate;
		//}
		//else {
		//	// Choose Q value second good
		//	vector<double> tmp;
		//	for (auto it = scheduling.job_cadidate.begin(); it != scheduling.job_cadidate.end(); ++it) {
		//		tmp.push_back(q_table[it->second.job_id][it->second.ope_id]);
		//	}
		//	int index = min_element(tmp.begin(), tmp.end()) - tmp.begin();
		//	auto it = scheduling.job_cadidate.begin();
		//	advance(it, index);//一次跳 index 距離
		//	return Candidate{ it->second.job_id, it->second.ope_id, it->second.remain_ops };
		//}
	}
}

void QLearning::UpdateReleaseTime(const Candidate& candidate) {
	// Get Candidate Info
	auto it_machine = scheduling.demands.at(candidate.job_id).get_operations().at(candidate.ope_id).get_available_machines().at(0); // 只有一台 machine
	int machine_id = it_machine.get_machine_id();
	double process_time = it_machine.get_process_time();

	// start_time = max(job_release_time, machine_release_time); end_time = start_time + process_time
	double start_time = 0.0, end_time = 0.0;
	if (scheduling.job_release_time.at(candidate.job_id) >= scheduling.machine_release_time.at(machine_id)) {
		start_time = scheduling.job_release_time.at(candidate.job_id);
	}
	else {
		start_time = scheduling.machine_release_time.at(machine_id);
	}
	end_time = start_time + process_time;

	// Update job_release_time, machine_release_time
	scheduling.job_release_time.at(candidate.job_id) = end_time;
	scheduling.machine_release_time.at(machine_id) = end_time;

	// Write into scheduled_tasks
	vector<int> machine_ids { machine_id };
	vector<double> process_times { process_time };
	ScheduledTask scheduled_task{ candidate.job_id, candidate.ope_id,  machine_ids,  process_times, start_time, end_time };
	scheduled_tasks.push_back(scheduled_task);
}

Reward QLearning::CalReward(vector<ScheduledTask>& scheduled_tasks, const Candidate& candidate) {
	
	Reward reward;
	
	// Calculate makespan = max (job_release_time, machine_release_time)
	auto max_job_release_time = get_max_map_value(scheduling.job_release_time);
	auto max_machine_release_time = get_max_map_value(scheduling.machine_release_time);
	double makespan = max(max_job_release_time.second, max_machine_release_time.second);
	reward.makespan = makespan;

	// 計算 space_bw_last_job
	if (candidate.ope_id == 0) {
		reward.space_bw_last_job = 0.0; // 第一道工序，所以不會有前一道空隙
	}
	else {
		int job_id = candidate.job_id;
		int pre_ope_id = candidate.ope_id - 1;
		for (auto it = scheduled_tasks.rbegin(); it != scheduled_tasks.rend(); ++it) {
			if (it->job_id == job_id && it->ope_id == pre_ope_id) {
				if (scheduled_tasks.back().start_time - it->end_time == 0) {
					reward.space_bw_last_job = 0;
					break;
				}
				else {
					reward.space_bw_last_job = scheduled_tasks.back().start_time - it->end_time;
					break;
				}	
			}
		}
	}

	// 計算 space_bw_last_machine
	int now_machine = scheduled_tasks.back().machine_id.at(0);
	int count = 0;
	reward.space_bw_last_machine = 0;
	for (auto it = scheduled_tasks.rbegin(); it != scheduled_tasks.rend(); ++it) {
		if (count == 1 && it->machine_id.at(0) == now_machine) {
			if (scheduled_tasks.back().start_time - it->end_time == 0) {
				break;
			}
			else {
				reward.space_bw_last_machine = scheduled_tasks.back().start_time - it->end_time;
				break;
			}
		}
		if (it->machine_id.at(0) == now_machine) {
			count++;
		}
	}

	// 計算 process time
	reward.process_time = scheduled_tasks.back().process_time.back();

	// 計算 job_release_time, machine_release_time
	reward.job_release_time = scheduling.job_release_time.find(candidate.job_id)->second;
	reward.machine_release_time = scheduling.machine_release_time.find(scheduled_tasks.back().machine_id.back())->second;

	return reward;
}

void QLearning::UpdateJobCandidate(const Candidate& candidate) {
	auto it = scheduling.job_cadidate.find(candidate.job_id);
	if (it != scheduling.job_cadidate.end()) {
		if (it->second.remain_ops == 1) {// 表示最後一道，移除候選人
			scheduling.job_cadidate.erase(it->first);
		}else{
			it->second.ope_id++;
			it->second.remain_ops--;
		}
	}
	else {
		cout << "Scheduling Error ! Please Check Program." << endl;
	}

}

double QLearning::GetNextStatesMaxQValue(const Candidate& candidate) {
	// 假設該次選到的點是 job_id = 1, ope_id = 1，那他下一個可能的狀態只會是 job_id = 1, ope_id = 2
	// 這裡不用管 Machine，因為每個 (job,op) 只有一個機台可以做
	double maxQ = 0.0;
	if (candidate.remain_ops != 1) {
		maxQ = q_table[candidate.job_id][candidate.ope_id + 1];
	}
	return maxQ;
};


void QLearning::UpdateQTable(const Candidate& candidate, const Reward& reward) {
	
	// Reward 
	double sum_reward = (1 / (reward.machine_release_time + reward.job_release_time));
	//double sum_reward = (1 / reward.machine_release_time) + (1 / reward.job_release_time);
	//double sum_reward = (1 / reward.makespan);
	//double sum_reward = (1 / reward.space_bw_last_job);
	//double sum_reward = 0.0;
	//vector<double> rewards;
	//rewards.push_back(1 / reward.machine_release_time); rewards.push_back(1 / reward.job_release_time);
	//rewards.push_back(1 / (reward.space_bw_last_machine + 1)); rewards.push_back(1 / (reward.space_bw_last_job + 1));
	//rewards.push_back(1 / reward.makespan);
	//double min = *min_element(rewards.begin(), rewards.end());
	//double max = *max_element(rewards.begin(), rewards.end());
	//for (auto it = rewards.begin(); it != rewards.end(); ++it) {
	//	*it = 2 * (*it - min) / (max - min) - 1;
	//}
	//for (auto it = rewards.begin(); it != rewards.end(); ++it) {
	//	//*it /= *it  / (1 + abs(*it));
	//	//*it = tan(*it);
	//}
	//std::for_each(rewards.begin(), rewards.end(), [&](int n) {
	//	sum_reward += n;
	//});

	// Update Q table => Q = (1-alpha)*Q_old + alpha*Q_current。
	if (scheduling.job_cadidate.size() != 0) {
		/*q_table[candidate.job_id][candidate.ope_id] += (1 - alpha) *  q_table[candidate.job_id][candidate.ope_id] +
			alpha * (sum_reward + gamma * GetNextStatesMaxQValue());*/
		q_table[candidate.job_id][candidate.ope_id] += alpha * (sum_reward + gamma * GetNextStatesMaxQValue(candidate) - q_table[candidate.job_id][candidate.ope_id]);
	}
	else {
		/*q_table[candidate.job_id][candidate.ope_id] += (1 - alpha) *  q_table[candidate.job_id][candidate.ope_id] +
			alpha * (sum_reward + gamma * 0);*/
		q_table[candidate.job_id][candidate.ope_id] += alpha * sum_reward;
	}
}


void QLearning::start() {	
	int sum_op_ops = scheduling.get_sum_of_operations();
	for (int j = 0; j < sum_op_ops; ++j) {
		// Find one candidate => 查詢 Q-table 來選擇，但有 epislon = 20% 機率隨機選擇
		Candidate candidate = GetJobCandidate();

		// Update job_release_time & machine_release_time & write into scheduled_task
		UpdateReleaseTime(candidate);

		// Calculate Reward
		Reward reward = CalReward(scheduled_tasks, candidate);

		// Update job_cadidate
		UpdateJobCandidate(candidate);

		// Update Q Table
		UpdateQTable(candidate, reward);
	}
}


void QLearning::record() {
	// Record scheduled_tasks
	//all_scheduled_tasks.push_back(scheduled_tasks);
	// Record makespan
	auto max_job_release_time = get_max_map_value(scheduling.job_release_time);
	auto max_machine_release_time = get_max_map_value(scheduling.machine_release_time);
	double makespan = max(max_job_release_time.second, max_machine_release_time.second);
	makespans.push_back(makespan);
}

void QLearning::reset() {
	scheduling.Initialize_Job_Cadidate();
	scheduling.Initialize_Job_Release_Time();
	scheduling.Initialize_Machine_Release_Time();
	scheduled_tasks.clear();
}

Result QLearning::test() {
	vector<vector<double>> best_q_table = get_best_q_table();
	int sum_op_ops = scheduling.get_sum_of_operations();
	Candidate candidate;
	for (int j = 0; j < sum_op_ops; ++j) {
		// Get all candidate
		double max_q_value = 0;
		for (auto it = scheduling.job_cadidate.begin(); it != scheduling.job_cadidate.end(); ++it) {
			// Find max value in Q table => Q(St, at), 其中 Q 的策略是 argmax Q(s,a) : 在狀態 s 時採取能獲得最大回報的動作		
			if (best_q_table[it->second.job_id][it->second.ope_id] > max_q_value) {
				candidate.job_id = it->second.job_id;
				candidate.ope_id = it->second.ope_id;
				candidate.remain_ops = it->second.remain_ops;
				max_q_value = best_q_table[it->second.job_id][it->second.ope_id];
			}
		}
		// Update job_release_time & machine_release_time & write into scheduled_task
		UpdateReleaseTime(candidate);

		// Update job_cadidate
		UpdateJobCandidate(candidate);
	}
	auto max_job_release_time = get_max_map_value(scheduling.job_release_time);
	auto max_machine_release_time = get_max_map_value(scheduling.machine_release_time);
	double makespan = max(max_job_release_time.second, max_machine_release_time.second);
	return Result{ makespan };
}
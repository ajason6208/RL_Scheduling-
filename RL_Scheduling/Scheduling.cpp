#include "Scheduling.h"

// 覆載 Job 輸入運算子
istream&  operator>> (istream& in, Job& job) {
	
	vector<pair<int, double>> machine_contents; // int : machine_id, double : machine process time
	pair<int, double> content;
	while (in >> content.first >> content.second) {
		machine_contents.push_back(content);
	}

	// Num of operation in one row
	vector<Operation> operations;
	for (auto it = machine_contents.begin(); it != machine_contents.end(); ++it) {	
		vector<Machine> machines;
		machines.push_back(Machine(it->first, it->second));
		operations.push_back(Operation(static_cast<int>(it - machine_contents.begin()), machines));
	}
	
	job.set_operations(operations);
	
	return in;
}

// Initialize Variable
void Scheduling::Initialize_Job_Release_Time() {
	job_release_time.clear();
	for (auto it = demands.begin(); it != demands.end(); ++it) {
		job_release_time.insert(make_pair(it->get_job_id(), 0.0));	
	}
}
void Scheduling::Initialize_Machine_Release_Time() {
	machine_release_time.clear();
	vector<int> unique_machine_id;
	for (auto it = demands.begin(); it != demands.end(); ++it) {
		for (auto it2 = it->get_operations().begin(); it2 != it->get_operations().end(); ++it2) {
			for (auto it3 = it2->get_available_machines().begin(); it3 != it2->get_available_machines().end(); ++it3) {
				unique_machine_id.push_back(it3->get_machine_id());
			}
		}
	}
	auto unique_it = unique(unique_machine_id.begin(), unique_machine_id.end());
	unique_machine_id.resize(distance(unique_machine_id.begin(), unique_it));
	for (auto it = unique_machine_id.begin(); it != unique_machine_id.end(); ++it) {
		machine_release_time.insert(make_pair(*it, 0.0));
	}
}

void Scheduling::Initialize_Job_Cadidate() {
	for (auto it = demands.begin(); it != demands.end(); ++it) {
		Candidate candidate{ it->get_job_id(), 0, static_cast<int>(it->get_operations().size()) };
		job_cadidate.insert(make_pair(it->get_job_id(), candidate));
	}
}

void Scheduling::Initialize_Sum_Of_Ops() {
	this->sum_of_operations = Get_Sum_Ops();
}

Scheduling::Scheduling() {

}

Scheduling::Scheduling(const string& file_name, const int& index) {
	if (index == 1) {// or-lib	
		ifstream infile(file_name);
		string line;

		Job job;
		istringstream  entry;

		// Read or-lib data
		int count_job_id = 0;
		while (getline(infile, line)) {
			entry.str(line);
			entry >> job;
			job.set_job_id(count_job_id);
			count_job_id++;
			demands.push_back(job);
			entry.clear();
		}

		// Initialize job_release_time & machine_release_time
		Initialize_Job_Release_Time();
		Initialize_Machine_Release_Time();

		// Initialize Candidate variable
		Initialize_Job_Cadidate();

		// Initialize Sum Of Operations
		Initialize_Sum_Of_Ops();
	}
}

Scheduling::~Scheduling() {

}

// set private variable
void Scheduling::set_demands(const Job& job) {
	this->demands.push_back(job);
}

void Scheduling::set_job_cadidate(const int& job_id, const int& ope_id) {
	auto it = job_cadidate.find(job_id);
	if (it != job_cadidate.end()) {
		it->second.ope_id = ope_id;
	}
	else {
		system("pause");
	}
}

// get private variable
int Scheduling::get_sum_of_operations() {
	return this->sum_of_operations;
}

// Get Function
int Scheduling::Get_Sum_Ops() { // 計算該次排成需要排的全部 operations 數量
	int temp = 0;
	for (auto it = demands.begin(); it != demands.end(); ++it) {
		temp += static_cast<int>(it->get_operations().size());
	}
	return temp;
}
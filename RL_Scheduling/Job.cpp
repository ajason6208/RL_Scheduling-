#include "Job.h"


Job::Job() {

}
Job::~Job() {

}

// set private variable
void Job::set_job_id(const int& job_id){
	this->job_id = job_id;
}
void Job::set_operations(const vector<Operation>& operations) {
	this->operations = operations;
}

// get private variable
int Job::get_job_id() {
	return this->job_id;
}

vector<Operation>& Job::get_operations() {
	return this->operations;
}
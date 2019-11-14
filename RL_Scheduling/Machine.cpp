#include "Machine.h"


Machine::Machine(const int& machine_id, const double& process_time) {
	this->machine_id = machine_id;
	this->process_time = process_time;
}
Machine::~Machine() {

}

// get private variable
int& Machine::get_machine_id() {
	return this->machine_id;
}

double& Machine::get_process_time() {
	return this->process_time;
}

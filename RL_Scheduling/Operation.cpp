#include "Operation.h"



Operation::Operation(const int& ope_id, const vector<Machine>& available_machines) {
	this->ope_id = ope_id;
	this->available_machines = available_machines;
}
Operation::~Operation() {

}

// set private variable
void Operation::set_available_machines(const Machine& machine) {
	this->available_machines.push_back(machine);
}

// get private variable
vector<Machine>& Operation::get_available_machines() {
	return this->available_machines;
}
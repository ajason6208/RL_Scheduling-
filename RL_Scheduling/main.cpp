#include <iostream> 

#include "Scheduling.h"
#include "FileProcess.h"
#include "config.h"
#include "QLearning.h"

using namespace std;

int main(){

	// Load data & Initialize Variable
	FileProcess process;
	vector<string> all_files = process.Get_All_Files(OR_LIB_FILE_PATH);
	for (auto it = all_files.begin(); it != all_files.end(); ++it) {
		Scheduling scheduling((OR_LIB_FILE_PATH + *it), 1);

		// Q Learning
		QLearning qlearning(scheduling);
		qlearning.episode = scheduling.get_sum_of_operations();
		for (int i = 0; i < qlearning.episode; ++i) {			
			// Start Q learning
			qlearning.start();
			
			// Record Result
			qlearning.record();

			// Reset q learning Variable & 
			qlearning.reset();	

			// Record Use Best Parameter 
			if (qlearning.makespans.back() < qlearning.best_makespan) {
				qlearning.best_makespan = qlearning.makespans.back();
				qlearning.best_q_tables = qlearning.get_q_table();
			}
			else if (qlearning.makespans.back() * qlearning.denoise > qlearning.best_makespan) { // 如果差到 denoise 以上，直接用最好的取代
				qlearning.set_q_table(qlearning.best_q_tables);
			}
		}	

		// Show best Result
		cout << *it << ": " << *min_element(qlearning.makespans.begin(), qlearning.makespans.end()) << endl;

		// Write Makespan Result
		fstream fp;
		string file_name = "result.csv";
		fp.open(file_name, ios::app);
		/*for (auto it = qlearning.makespans.begin(); it != qlearning.makespans.end(); ++it) {
			fp << *it << endl;
		}*/
		fp << *it << "," << *min_element(qlearning.makespans.begin(), qlearning.makespans.end()) << endl;
		fp.close();
	}
	system("pause");
}

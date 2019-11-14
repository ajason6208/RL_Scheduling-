#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>

#include "FileProcess.h"

using namespace boost::filesystem;
using namespace std;


vector<string> FileProcess::Get_All_Files(string folder_path) {

	vector<string> all_files_name;
	path p(folder_path);

	for (auto i = directory_iterator(p); i != directory_iterator(); i++)
	{
		if (!is_directory(i->path())) //we eliminate directories
		{
			all_files_name.push_back(i->path().filename().string());
		}
		else
			continue;
	}
	return all_files_name;
}

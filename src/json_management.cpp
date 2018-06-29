#include "json_management.h"

void write_json_file(std::vector<std::string> keys, std::vector<double> values, std::string path)
{
	std::ofstream outfile (path);
	outfile << "{";

	for(unsigned int i = 0; i < keys.size()-1; ++i)
		outfile << "\"" << keys[i] << "\": " << values[i] << ",";

	outfile << "\"" << keys[keys.size()-1] << "\": " << values[keys.size()-1] << "}";	
	outfile.close();

}
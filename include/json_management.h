#ifndef Json_H
#define Json_H

#include <string>
#include <vector>
#include <fstream>

template <class T>
void write_json_file(std::vector<std::string> keys, std::vector<T> values, std::string path)
{
	std::ofstream outfile (path);
	outfile << "{";

	for(unsigned int i = 0; i < keys.size()-1; ++i)
		outfile << "\"" << keys[i] << "\": " << values[i] << ",";

	outfile << "\"" << keys[keys.size()-1] << "\": " << values[keys.size()-1] << "}";	
	outfile.close();
}

#endif
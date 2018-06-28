#ifndef Json_H
#define Json_H

#include <string>
#include <vector>
#include <fstream>

void write_json_file(std::vector<std::string> keys, std::vector<double> values, std::string path);

#endif
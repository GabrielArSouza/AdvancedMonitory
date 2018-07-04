#include "monitor.h"

void general_disk_usage()
{
	std::cout << "retrieving general disk usage...\n";
	std::string script = "df\n";
	std::string result = pegar_saida_comando(script.c_str());

	std::istringstream sstream(result);
	std::string line;
	std::string trash;

	// skips first line (headers)
	getline(sstream, line);

	long total_used = 0;
	long total_not_used = 0;
	long used;
	long not_used;

	while(getline(sstream, line))
	{
		std::istringstream sstream_line(line);

		// Filesystem
		sstream_line >> trash; 

		// 1K-Blocks
		sstream_line >> trash; 

		sstream_line >> used >> not_used;
		total_used += used;
		total_not_used += not_used;
	}

	std::vector<std::string> labels;
	labels.push_back("livre");
	labels.push_back("usada");
	std::vector<long> data;	
	data.push_back(total_not_used / (1024*1024));
	data.push_back(total_used / (1024*1024));
	write_json_file(labels, data, "data/general_disk_data.json");

	std::cout << "done.\n";
}

/**
 * @brief      monitors data from RAM
 */
void general_memory_usage()
{
	std::cout << "retrieving general memory usage...\n";
	std::string script = "cat /proc/meminfo\n";
	std::string result = pegar_saida_comando(script.c_str());

	// stream for read "result" variable more easy
	std::istringstream stm(result);

	// stores the read words
	std::string trash;

	// the stream read numbers
	double number;

	std::vector<double> dados;	
	std::vector<std::string> labels;
	labels.push_back("total");
	labels.push_back("livre");
	labels.push_back("usada");


	for ( int i=0; i < 3; i++ )
	{
		stm >> trash;
		stm >> number;
		stm >> trash;
		std::setprecision(2);
		dados.push_back(number/(1024*1024));
	}

	write_json_file(labels, dados, "data/general_memory_data.json");

	std::cout << "done.\n";
}

void data_per_process(std::string output_path)
{
	std::cout << "fetching data per process...\n";

	std::ofstream outfile(output_path);
	outfile << "[";

	// all folders in /proc whose name is a number
	std::string cmd_current_procs = "find /proc/*  -maxdepth 0 -type d -regextype sed -regex \"^/proc/[0-9]*$\"";
	std::string current_procs = pegar_saida_comando(cmd_current_procs.c_str());

	std::istringstream sstream_procs(current_procs);
	std::string proc_path;

	std::vector<std::string> labels;
	std::vector<double> values;
	std::vector<int> pids;

	while(sstream_procs >> proc_path)
		pids.push_back(stoi(proc_path.substr(6)));

	std::sort(pids.begin(), pids.end());

	// iterates over all processes
	for(int pid : pids)
	{
		outfile << "{\"pid\": " << pid << ",";

		// gets the status of that process
		std::string cmd_proc_status = "cat /proc/" + std::to_string(pid) + "/status";
		std::string proc_status = pegar_saida_comando(cmd_proc_status.c_str());

		std::istringstream sstream_proc_status(proc_status);
		std::string word;

		bool hasVmPeak = false;
		bool hasVmSize = false;
		bool hasVmLck = false;
		bool hasVmPin = false;
		bool hasVmHWM = false;
		bool hasVmRSS= false;

		while(sstream_proc_status >> word)
		{
			if(word == "VmPeak:")
			{
				outfile << "\"vmpeak\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
				hasVmPeak = true;
			}

			else if(word == "VmSize:")
			{
				outfile << "\"vmsize\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
				hasVmSize = true;
			}

			else if(word == "VmLck:")
			{
				outfile << "\"vmlck\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
				hasVmLck = true;
			}

			else if(word == "VmPin:")
			{
				outfile << "\"vmpin\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
				hasVmPin = true;
			}

			else if(word == "VmHWM:")
			{
				outfile << "\"vmhwm\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
				hasVmHWM = true;
			}

			else if(word == "VmRSS:")
			{
				outfile << "\"vmrss\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
				hasVmRSS = true;

				break;
			}
		}

		if(!hasVmPeak) outfile << "\"vmpeak\": \"-\",";
		if(!hasVmSize) outfile << "\"vmsize\": \"-\",";
		if(!hasVmLck) outfile << "\"vmlck\": \"-\",";
		if(!hasVmPin) outfile << "\"vmpin\": \"-\",";
		if(!hasVmHWM) outfile << "\"vmhwm\": \"-\",";
		if(!hasVmRSS) outfile << "\"vmrss\": \"-\",";

		// replaces the last char (which is a comma) for a closing bracket
		char last_char = '}';
		outfile.seekp(-1,std::ios::end);
		outfile.write(reinterpret_cast<char*>(&last_char),sizeof(last_char));
		outfile << ",";

	}

	// replaces the last char (which is a comma) for a closing squared bracket
	char last_char = ']';
	outfile.seekp(-1,std::ios::end);
	outfile.write(reinterpret_cast<char*>(&last_char),sizeof(last_char));
	outfile.close();
}
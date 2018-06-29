#include "monitor.h"

/**
 * @brief      monitors data from RAM
 */
void toMonitor ()
{
	std::cout << "chamou monitor\n";
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

	write_json_file(labels, dados, "data/data.json");
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

	// iterates over all processes
	while(sstream_procs >> proc_path)
	{
		std::cout << "fetching data of process: " << proc_path << std::endl;
		std::string pid = proc_path.substr(6);
		outfile << "{\"pid\": " << pid << ",";

		// gets the status of that process
		std::string cmd_proc_status = "cat " + proc_path + "/status";
		std::string proc_status = pegar_saida_comando(cmd_proc_status.c_str());

		std::istringstream sstream_proc_status(proc_status);
		std::string word;


		while(sstream_proc_status >> word)
		{
			if(word == "VmPeak:")
			{
				outfile << "\"vmpeak\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
			}

			else if(word == "VmSize:")
			{
				outfile << "\"vmsize\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
			}

			else if(word == "VmLck:")
			{
				outfile << "\"vmlck\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
			}

			else if(word == "VmPin:")
			{
				outfile << "\"vmpin\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
			}

			else if(word == "VmHWM:")
			{
				outfile << "\"vmhwm\":";
				sstream_proc_status >> word;		
				outfile << word << ",";
			}

			else if(word == "VmRSS:")
			{
				outfile << "\"vmrss\":";
				sstream_proc_status >> word;		
				outfile << word << ",";

				break;
			}
		}

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
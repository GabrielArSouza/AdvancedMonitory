#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "monitor.h"

int main ()
{
	std::cout << "Running application\n";
	std::cout << "Press 'Ctrl+C' to exit\n";
	
	while ( true )
	{
		general_memory_usage();
		general_disk_usage();
		data_per_process("data/per_process.json");
		sleep(1);
	}

	return 0;
}
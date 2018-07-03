#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "terminal.h"
#include "json_management.h"
#include <algorithm> // sort
#include <string>
#include <iostream>
#include <sstream>    // istringstream
#include <stdio.h>
#include <vector>
#include <iomanip>      // std::setprecision
/**
 * @brief      monitors data from RAM
 */
void general_memory_usage();

void data_per_process(std::string output_path);

void general_disk_usage();
#endif
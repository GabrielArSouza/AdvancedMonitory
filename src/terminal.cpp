#include "terminal.h"

std::string pegar_saida_comando(const char* comando)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(comando, "r");

    if (!pipe)
    {
    	throw std::runtime_error("popen() failed!");	
    }

    try
    {
        while (!feof(pipe)) 
        {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } 

    catch (...) {
        pclose(pipe);
        throw;
    }

    pclose(pipe);

    return result;
}
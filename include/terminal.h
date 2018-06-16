#ifndef Terminal_h
#define Terminal_h

#include <stdexcept>
#include <string>

// executa um comando no terminal e retorna uma string contendo
// a saída do comando. baseada em: 
// https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
std::string pegar_saida_comando(const char* cmd);

#endif
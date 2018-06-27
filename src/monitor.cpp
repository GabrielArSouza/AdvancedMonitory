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


	for ( int i=0; i < 3; i++ )
	{
		stm >> trash;
		stm >> number;
		stm >> trash;
		std::setprecision(2);
		dados.push_back(number/(1024*1024));
	}

	FILE *arq = fopen("/home/gabriel/Códigos/AdvancedMonitory/IO/dados.txt", "wt");

	if(arq == NULL)
	{
		std::cout << "Erro: Arquivo não encontrado\n";
	 	exit(0);
	}

	for ( unsigned int i=0; i < dados.size(); i++)
	{
		std::ostringstream strs;
		strs << dados[i] << " | ";
		std::string str = strs.str();

		auto result = fputs(str.c_str(), arq);	
		if (result == 'EOF')
    		std::cout << "Erro na Gravacao\n";
	}
	
	fclose(arq);
}
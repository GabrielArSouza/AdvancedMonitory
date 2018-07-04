#include <cstdlib>
#include <unistd.h>
#include <vector>

int main()
{
	std::vector<long> v;
	
	for(long i = 0; i < 100000000000; ++i)
	{
		v.push_back(i);
		//sleep(1);
		
	}

	return 0;
}
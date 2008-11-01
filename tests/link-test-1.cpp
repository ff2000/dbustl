#include <dbustl-1/dbustl>

#include <cstdlib>

void testMessage1(dbustl::Message& m)
{
	std::vector<std::string> v;
	
	m >> v;
	m << v;
}

void testMessage2(dbustl::Message& m);

int main()
{
	//None of this code is supposed to be executed, it's just 
	//pure compile time test
	exit(0);
	
	dbustl::Message m(0);
	testMessage1(m);
	testMessage2(m);
	return 0;
}

#include <dbustl-1/dbustl>

void testMessage2(dbustl::Message& m)
{
	std::vector<std::string> v;
	
	m >> v;
	m << v;
}

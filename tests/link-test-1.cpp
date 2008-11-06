#include <dbustl-1/dbustl>

#include <cstdlib>
#include <string>

void testMessage1(dbustl::Message& m)
{	
	{
		bool t;
		m >> t;
		m << t;
	}
	
	{
		char t;
		//Note : deserialization does not exist for signed char due to lack of 
		//adequate DBUS representation
		m << t;
	}
	
	{
		unsigned char t;
		m >> t;
		m << t;
	}
	
	{
		signed char t;
		//Note : deserialization does not exist for signed char due to lack of 
		//adequate DBUS representation
		m << t;
	}
	
	{
		short t;
		m >> t;
		m << t;
	}
	
	{
		unsigned short t;
		m >> t;
		m << t;
	}
	
	{
		signed short t;
		m >> t;
		m << t;
	}
	
	{
		int t;
		m >> t;
		m << t;
	}
	
	{
		unsigned int t;
		m >> t;
		m << t;
	}
	
	{
		signed int t;
		m >> t;
		m << t;
	}
	
	{
		long t;
		m >> t;
		m << t;
	}
	
	{
		unsigned long t;
		m >> t;
		m << t;
	}
	
	{
		signed long t;
		m >> t;
		m << t;
	}
	
	{
		long long t;
		m >> t;
		m << t;
	}
	
	{
		unsigned long long t;
		m >> t;
		m << t;
	}
	
	{
		signed long long t;
		m >> t;
		m << t;
	}
	
	{
		float t;
		//Note : deserialization does not exist for float due to lack of 
		//adequate DBUS representation
		m << t;
	}
	
	{
		double t;
		m >> t;
		m << t;
	}	
	
	{
		const char * t;
		//Note : deserialization does not exist;
		m << t;
	}
	
	{
		char * t;
		//Note : deserialization does not exist;
		m << t;
	}
	
	{
		std::string t;
		m >> t;
		m << t;
	}
	{
		std::vector<std::string> v;
		m >> v;
		m << v;
	}
	{
		std::list<std::string> v;
		m >> v;
		m << v;
	}
	{
		std::set<std::string> v;
		m >> v;
		m << v;
	}
	{
		std::map<double, std::string> v;
		m >> v;
		m << v;
	}
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


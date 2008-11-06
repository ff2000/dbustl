#include <dbustl-1/Message>

#include <dbustl-1/types/list>
#include <dbustl-1/types/vector>
#include <dbustl-1/types/set>
#include <dbustl-1/types/map>

void testMessage2(dbustl::Message& m)
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
		std::multiset<std::string> v;
		m >> v;
		m << v;
	}
	{
		std::map<double, std::string> v;
		m >> v;
		m << v;
	}
}

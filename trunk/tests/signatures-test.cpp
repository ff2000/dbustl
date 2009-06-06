#include <dbustl-1/dbustl>

#include <iostream>
#include <cassert>

int main()
{
	assert(std::string("as") == dbustl::types::Signature<std::vector<std::string> >());
	assert(std::string("as") == dbustl::types::Signature<std::list<std::string> >());
	assert(std::string("as") == dbustl::types::Signature<std::set<std::string> >());
	assert(std::string("as") == dbustl::types::Signature<std::multiset<std::string> >());
	assert((std::string("{ds}") == dbustl::types::Signature<std::pair<const double, std::string> >()));
	assert((std::string("{sad}") == dbustl::types::Signature<std::pair<const std::string, std::list<double> > >()));
	std::cout << "Ok" << std::endl;
	return 0;
}


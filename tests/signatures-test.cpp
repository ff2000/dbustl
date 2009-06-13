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
#ifdef DBUSTL_CXX0X
	assert((std::string("as") == dbustl::types::Signature<std::array<std::string, 5> >()));
	assert((std::string("(iidii)") == dbustl::types::Signature<std::tuple<int, int, double, int, int> >()));
	assert(std::string("as") == dbustl::types::Signature<std::unordered_set<std::string> >());
	assert(std::string("as") == dbustl::types::Signature<std::unordered_multiset<std::string> >());
#endif
	std::cout << "Ok" << std::endl;
	return 0;
}


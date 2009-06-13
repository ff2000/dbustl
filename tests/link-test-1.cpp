#include <dbustl-1/dbustl>

#include <cstdlib>
#include <string>

template<typename _Tp>
    struct myLess : public std::binary_function<_Tp, _Tp, bool>
    {
        bool operator()(const _Tp& __x, const _Tp& __y) const
        { 
        	return __x < __y;
        }
    };
    
template <class T>
class myAllocator
{
public:
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef T         value_type;

  myAllocator() {}
  myAllocator(const myAllocator&) {}



  pointer   allocate(size_type n, const void * = 0) {
              T* t = (T*) malloc(n * sizeof(T));
              return t;
            }
  
  void      deallocate(void* p, size_type) {
              if (p) {
                free(p);
              } 
            }

  pointer           address(reference x) const { return &x; }
  const_pointer     address(const_reference x) const { return &x; }
  myAllocator<T>&  operator=(const myAllocator&) { return *this; }
  void              construct(pointer p, const T& val) 
                    { new ((T*) p) T(val); }
  void              destroy(pointer p) { p->~T(); }

  size_type         max_size() const { return size_t(-1); }

  template <class U>
  struct rebind { typedef myAllocator<U> other; };

  template <class U>
  myAllocator(const myAllocator<U>&) {}

  template <class U>
  myAllocator& operator=(const myAllocator<U>&) { return *this; }
};
    
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
		float t = 0;
		//Note : deserialization does not exist for float due to lack of 
		//adequate DBUS representation
		m << t;
	}
	
	{
		double t = 0;
		m >> t;
		m << t;
	}	
	
	{
		const char * t = "";
		//Note : deserialization does not exist;
		m << t;
	}
	
	{
		//Note : deserialization does not exist;
		m << "test";
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
	{
		std::map<std::string, std::list<double> > v;
		m >> v;
		m << v;
	}
	{
		std::multimap<double, std::string> v;
		m >> v;
	}
#ifdef DBUSTL_CXX0X
	{
		std::tuple<int, int> v;
		m << v;
		m >> v;
	}
	{
		std::array<int, 5> v;
		m << v;
		m >> v;
	}
	{
		std::unordered_set<std::string> v;
		m >> v;
		m << v;
	}
	{
		std::unordered_multiset<std::string> v;
		m >> v;
		m << v;
	}
	{
		std::unordered_map<double, std::string> v;
		m >> v;
		m << v;
	}
	{
		std::unordered_multimap<double, std::string> v;
		m >> v;
	}
#endif
	
	/* Now test standard containers with non standard comparison operations
	 * and allocators - we test with map to also instantiate the Signature */
	{
		std::map<int, std::list<std::string, myAllocator<std::string> > > v;
		m >> v;
		m << v;
	}
	{
		std::map<int, std::vector<std::string, myAllocator<std::string> > > v;
		m >> v;
		m << v;
	}
	{
		std::map<int, std::set<std::string, myLess<std::string>, myAllocator<std::string> > > v;
		m >> v;
		m << v;
	}
	{
		std::map<int, std::multiset<std::string, myLess<std::string>, myAllocator<std::string> > > v;
		m >> v;
		m << v;
	}
	{
		std::map<int, std::string, myLess<int>, myAllocator<std::pair<const int, std::string> > > v;
		m >> v;
		m << v;
	}
	{
		std::multimap<int, std::string, myLess<int>, myAllocator<std::pair<const int, std::string> > >  v;
		m >> v;
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


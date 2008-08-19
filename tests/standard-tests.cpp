#include <dbustl.h>

#include <iostream>
#include <string>
#include <cassert>

int main()
{    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    bool exception_thrown;
    
    try {
        std::cout << ">Basic procedure call" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::string message;
        pythonServerProxy.call("SimpleProc"); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">String parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::string message;
        pythonServerProxy.call("SimpleHello", std::string("Hi"), &message); 
        assert(message == "Hi");            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">char* parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::string message;
        pythonServerProxy.call("SimpleHello", "Hi", &message); 
        assert(message == "Hi");            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">bool parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        bool value;
        pythonServerProxy.call("test_boolean", false, &value); 
        assert(value == true);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">byte parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        unsigned int in = 254;
        unsigned int out;
        pythonServerProxy.call("test_byte", in, &out); 
        assert(in == (out - 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">uint16 parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        uint16_t in = 65534;
        uint16_t out;
        pythonServerProxy.call("test_uint16", in, &out); 
        assert(in == (out - 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">int16 parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        int16_t in = 0;
        int16_t out;
        pythonServerProxy.call("test_int16", in, &out); 
        assert(in == (out + 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">uint32 parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        uint32_t in = (2^32) - 1;
        uint32_t out;
        pythonServerProxy.call("test_uint32", in, &out); 
        assert(in == (out - 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">int32 parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        int32_t in = 0;
        int32_t out;
        pythonServerProxy.call("test_int32", in, &out); 
        assert(in == (out + 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">uint64 parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        uint64_t in = (2^64) - 1;
        uint64_t out;
        pythonServerProxy.call("test_uint64", in, &out); 
        assert(in == (out - 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">int64 parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        int64_t in = 0;
        int64_t out;
        pythonServerProxy.call("test_int64", in, &out); 
        assert(in == (out + 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">unsigned long parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        unsigned long in = (2^32) - 1;
        unsigned long out;
        pythonServerProxy.call("test_uint32", in, &out); 
        assert(in == (out - 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">long parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        long in = 0;
        long out;
        pythonServerProxy.call("test_int32", in, &out); 
        assert(in == (out + 1));            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">double parameter passing" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        double in = 1.11;
        double out;
        pythonServerProxy.call("test_double", in, &out); 
        assert(in == out);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">float promotion to double " << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        float in = 1.11;
        double out;
        pythonServerProxy.call("test_double", in, &out); 
        assert(in == out);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">Array of string " << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::list<std::string> in, out;
        in.push_back("String 1");
        in.push_back("String 2");
        in.push_back("String 3");
        pythonServerProxy.call("test_array_of_string", in, &out); 
        assert(in == out);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">Array of int " << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::list<int32_t> in, out;
        in.push_back(1);
        in.push_back(2);
        in.push_back(3);
        pythonServerProxy.call("test_array_of_int", in, &out); 
        assert(in == out);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">Array of array of integers " << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::list<std::list<int32_t> > in, out;
        std::list<int32_t> l1, l2, l3;
        l1.push_back(1);
        l1.push_back(2);
        l2.push_back(3);
        l2.push_back(4);
        l3.push_back(5);
        l3.push_back(6);
        in.push_back(l1);
        in.push_back(l2);
        in.push_back(l3);

        pythonServerProxy.call("test_array_of_array_of_integer", in, &out); 
        assert(in == out);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">Array as vector" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::vector<std::vector<int32_t> > in, out;
        std::vector<int32_t> l1, l2, l3;
        l1.push_back(1);
        l1.push_back(2);
        l2.push_back(3);
        l2.push_back(4);
        l3.push_back(5);
        l3.push_back(6);
        in.push_back(l1);
        in.push_back(l2);
        in.push_back(l3);

        pythonServerProxy.call("test_array_of_array_of_integer", in, &out); 
        assert(in == out);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">Interface modifier" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        std::string message;
        pythonServerProxy.call("SimpleHello", 
          dbustl::ClientProxy::Interface("com.example.SampleInterface"), "Hi", &message); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Wrong return type test" << std::endl;
        exception_thrown = false;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        int message;
        pythonServerProxy.call("SimpleHello", "Hi", &message); 
    }
    catch(const std::exception& e) {
        if(e.what() == std::string("org.dbustl.ReturnParameterError: Unable to deserialize return parameter at index 0")) {
            exception_thrown = true;
        }
        else {
            std::cerr << e.what() << std::endl;
            return 1;
        }
    }
    if(!exception_thrown) {
        std::cerr << "No exception thrown !!" << std::endl;
        return 1;
    }
  
    return 0;
}

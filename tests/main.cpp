#include <dbustl.h>

#include <iostream>
#include <string>
#include <cassert>

int main()
{    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    
    try {
      std::cout << "Basic test" << std::endl;
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
      std::cout << "Interface modifier test" << std::endl;
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
      std::cout << "Wrong return type test" << std::endl;
      dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
      pythonServerProxy.setInterface("com.example.SampleInterface");
      int message;
      pythonServerProxy.call("SimpleHello", "Hi", &message); 
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }

    return 0;
}

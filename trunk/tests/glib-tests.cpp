#include <dbustl.h>
#include <dbustl/GlibMainLoopIntegration>

#include <iostream>
#include <functional>
#include <string>
#include <cassert>

class UserFunctorCallback : public std::binary_function<dbustl::Message, dbustl::DBusException, void> {
public:
    void operator()(const dbustl::Message&, const dbustl::DBusException&) {
        std::cout << "FunctorCallback: Call completed" << std::endl;
    };
};

class UserMethodCallback {
public:
    void method(const dbustl::Message&, const dbustl::DBusException&) {
        std::cout << "UserMethodCallback::method: Call completed" << std::endl;
    }
    void constMethod(const dbustl::Message&, const dbustl::DBusException&) const {
        std::cout << "UserMethodCallback::constMethod: Call completed" << std::endl;
    }
};

void userFunctionCallback(const dbustl::Message&, const dbustl::DBusException&) {
    std::cout << "functionCallback: Call completed" << std::endl;
}

int main()
{    
    dbustl::Connection::useMainLoop(dbustl::GlibMainLoopIntegration());    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    UserMethodCallback object;
    
    try {
        std::cout << ">Basis asynchronous call : Functor callback" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        pythonServerProxy.asyncCall("SimpleHello", UserFunctorCallback(), "Hi"); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">Basis asynchronous call : Method callback on non const object" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        pythonServerProxy.asyncCall("SimpleHello", &UserMethodCallback::method, &object, "Hi");
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Basis asynchronous call : function callback" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        pythonServerProxy.asyncCall("SimpleHello", &userFunctionCallback, "Hi"); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

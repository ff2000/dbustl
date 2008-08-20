#include <dbustl.h>
#include <dbustl/GlibMainLoopIntegration>

#include <iostream>
#include <functional>
#include <string>
#include <cassert>
#include <glibmm.h>

#include <dbus/dbus-glib-lowlevel.h>

Glib::RefPtr<Glib::MainLoop> mainloop;

class UserFunctorCallback : public std::binary_function<dbustl::Message, dbustl::DBusException, void> {
public:
    void operator()(dbustl::Message& m, const dbustl::DBusException& e) {
        std::string stringReturn;
        std::cout << "FunctorCallback: Call completed" << std::endl;
        assert(!e.isSet());
        m >> stringReturn;
        assert(stringReturn == "Hi");        
    };
};

class UserMethodCallback {
public:
    void method(dbustl::Message& m, const dbustl::DBusException& e) {
        std::string stringReturn;
        std::cout << "UserMethodCallback::method: Call completed" << std::endl;
        assert(!e.isSet());
        m >> stringReturn;
        assert(stringReturn == "Hi");        
    }
};

void userFunctionCallback(dbustl::Message& m, const dbustl::DBusException& e) {
    std::string stringReturn;
    std::cout << "functionCallback: Call completed" << std::endl;
    assert(!e.isSet());
    m >> stringReturn;
    assert(stringReturn == "Hi");        
}

void stopCallback(dbustl::Message&, const dbustl::DBusException&) {
    mainloop->quit();
}

int main()
{    
    dbustl::Connection::useMainLoop(dbustl::GlibMainLoopIntegration());    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    UserMethodCallback object;

    //FIXME : to be removed
    dbus_connection_setup_with_g_main(session->dbus(), NULL);
    
    //TODO: timeout handling

    mainloop = Glib::MainLoop::create();
    
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

    try {
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        pythonServerProxy.asyncCall("SimpleHello", &stopCallback); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    mainloop->run();
    mainloop.reset();
    return 0;
}

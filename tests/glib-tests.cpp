#include <dbustl/Glib>

#include <iostream>
#include <string>
#include <cassert>
#include <glibmm.h>

#include <dbus/dbus-glib-lowlevel.h>

Glib::RefPtr<Glib::MainLoop> mainloop;

//Number of times callbacks were called;
static unsigned int n_cbs = 0;

class UserFunctorCallback {
public:
    void operator()(dbustl::Message& m, const dbustl::DBusException& e) {
        std::string stringReturn;
        assert(!e.isSet());
        m >> stringReturn;
        assert(stringReturn == "Hi");        
        n_cbs++;
    };
};

class UserMethodCallback {
public:
    void method(dbustl::Message& m, const dbustl::DBusException& e) {
        std::string stringReturn;
        assert(!e.isSet());
        m >> stringReturn;
        assert(stringReturn == "Hi");        
        n_cbs++;
    }
};

void userFunctionCallback(dbustl::Message& m, const dbustl::DBusException& e) {
    std::string stringReturn;
    assert(!e.isSet());
    m >> stringReturn;
    assert(stringReturn == "Hi");        
    n_cbs++;
}

void inexistingMethodCallback(dbustl::Message&, const dbustl::DBusException& e) {
    assert(e.isSet());
    assert(e.name() == "org.freedesktop.DBus.Error.UnknownMethod");
    n_cbs++;
}

void sleepMethodCallback(dbustl::Message&, const dbustl::DBusException& e) {
    assert(e.isSet());
    assert(e.name() == "org.freedesktop.DBus.Error.NoReply");
    n_cbs++;
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
        std::cout << ">Interface modifier" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.asyncCall("SimpleHello", &userFunctionCallback,
          dbustl::ClientProxy::Interface("com.example.SampleInterface"), "Hi"); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Wrong method test" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        pythonServerProxy.asyncCall("InexistingMethod", &inexistingMethodCallback, 0, 1.0); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Timeout test" << std::endl;
        dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        pythonServerProxy.setTimeout(500);
        pythonServerProxy.asyncCall("test_sleep_2s", &sleepMethodCallback); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    //This last call is just o make sure that the mainloop finishes at some time
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
    
    assert(n_cbs == 6);
    
    return 0;
}

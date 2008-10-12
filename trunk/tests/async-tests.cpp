#include <dbustl/Glib>

#include <iostream>
#include <string>
#include <cassert>
#include <glib.h>

#include <dbus/dbus-glib-lowlevel.h>

GMainLoop *mainloop;

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

class UserFunctorCallbackNoReturn {
public:
    void operator()(dbustl::Message&, const dbustl::DBusException& e) {
        assert(!e.isSet());
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

class UserMethodCallbackNoReturn {
public:
    void method(dbustl::Message&, const dbustl::DBusException& e) {
        assert(!e.isSet());
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

void userFunctionCallbackNoReturn(dbustl::Message&, const dbustl::DBusException& e) {
    assert(!e.isSet());
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
    std::cout << "\tTimeout fired --> Ok.\n" << std::endl;
    n_cbs++;
}

void voidMethodCallback(dbustl::Message&, const dbustl::DBusException&) {
}

void stopMethodCallback(dbustl::Message&, const dbustl::DBusException&) {
    g_main_loop_quit(mainloop);
}

void exampleSignalCallback(dbustl::Message &m) {
    assert(m.member() == "exampleSignal");
    assert(m.interface() == "com.example.SampleInterface");
    n_cbs++;
}

class ExampleSignal2MethodCallback {
public:
    void method(dbustl::Message& m) {
        assert(m.member() == "exampleSignal2");
        assert(m.interface() == "com.example.SampleInterface");
        n_cbs++;
    }
};

class ExampleSignal3FunctorCallback {
public:
    void operator()(dbustl::Message& m) {
        assert(m.member() == "exampleSignal3");
        assert(m.interface() == "com.example.SampleInterface");
        n_cbs++;
    };
};

class ExampleSignalXFunctorCallback {
public:
    void operator()(dbustl::Message& m) {
        assert(m.interface() == "com.example.SampleInterface");
        n_cbs++;
    };
};

int main()
{    
    dbustl::Connection::useMainLoop(dbustl::GlibMainLoopIntegration());    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    UserMethodCallback object;
    ExampleSignal2MethodCallback object2;
    UserMethodCallbackNoReturn noreturnobject;
    unsigned int expected_cbs = 0;

    mainloop = g_main_loop_new(NULL, FALSE);
    //FIXME : remove dependancy on dbus-glib
    dbus_connection_setup_with_g_main(session->dbus(), NULL);
        
    dbustl::ClientProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
    pythonServerProxy.setInterface("com.example.SampleInterface");

    try {
        std::cout << ">0 arg asynchronous call : Functor callback" << std::endl;
        pythonServerProxy.asyncCall("SimpleProc", UserFunctorCallbackNoReturn());
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">0 arg asynchronous call : Method callback on non const object" << std::endl;
        pythonServerProxy.asyncCall("SimpleProc", &UserMethodCallbackNoReturn::method, &noreturnobject);
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">0 arg asynchronous call : function callback" << std::endl;
        pythonServerProxy.asyncCall("SimpleProc", &userFunctionCallbackNoReturn); 
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">1 arg asynchronous call : Functor callback" << std::endl;
        pythonServerProxy.asyncCall("SimpleHello", UserFunctorCallback(), "Hi");
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">1 arg asynchronous call : Method callback on non const object" << std::endl;
        pythonServerProxy.asyncCall("SimpleHello", &UserMethodCallback::method, &object, "Hi");
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">1 arg asynchronous call : function callback" << std::endl;
        pythonServerProxy.asyncCall("SimpleHello", &userFunctionCallback, "Hi"); 
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Interface modifier" << std::endl;
        pythonServerProxy.setInterface("BOGUS.INTERFACE");
        pythonServerProxy.asyncCall("SimpleHello", &userFunctionCallback,
          dbustl::ClientProxy::Interface("com.example.SampleInterface"), "Hi"); 
        pythonServerProxy.setInterface("com.example.SampleInterface");
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Wrong method test" << std::endl;
        pythonServerProxy.asyncCall("InexistingMethod", &inexistingMethodCallback, 0, 1.0); 
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Timeout test" << std::endl;
        pythonServerProxy.setTimeout(500);
        pythonServerProxy.asyncCall("test_sleep_2s", &sleepMethodCallback); 
        pythonServerProxy.setTimeout(-1);
        expected_cbs++; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Signal tests" << std::endl;
        pythonServerProxy.setSignalHandler("exampleSignal", &exampleSignalCallback); 
        pythonServerProxy.setSignalHandler("exampleSignal2", &ExampleSignal2MethodCallback::method, &object2); 
        pythonServerProxy.setSignalHandler("exampleSignal3", ExampleSignal3FunctorCallback()); 
        pythonServerProxy.setSignalHandler("", ExampleSignalXFunctorCallback()); 
        pythonServerProxy.asyncCall("SendSignals", &voidMethodCallback);
        //We expect 4 signals to be received
        expected_cbs += 4; 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    //This last call is just to make sure that the mainloop finishes at some time
    try {
        pythonServerProxy.asyncCall("SimpleProc", &stopMethodCallback); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    g_main_loop_run(mainloop);
    g_main_loop_unref(mainloop);
    
    assert(n_cbs == expected_cbs);
    
    return 0;
}

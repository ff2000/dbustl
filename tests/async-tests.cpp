/*
 *  DBusTL - DBus Template Library
 *
 *  Copyright (C) 2008  Fabien Chevalier <chefabien@gmail.com>
 *  
 *
 *  This file is part of the DBus Template Library.
 *
 *  The DBus Template Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  DBus Template Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with DBus Template Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <dbustl-1/dbustl>
#include <dbustl-1/GlibEventLoopIntegration>

#include <iostream>
#include <string>
#include <cassert>

#ifdef DBUSTL_NO_EXCEPTIONS
    #define TRY
    #define CATCH(ex, handler) if(pythonObjectProxy.hasError()) { ex = pythonObjectProxy.error(); handler }
#else
    #define TRY try
    #define CATCH(ex, handler) catch(ex) {handler}
#endif

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
    void method(dbustl::Message m) {
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
    dbustl::GlibEventLoopIntegration mli;
    dbustl::Connection::useEventLoop(mli);    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    ExampleSignal2MethodCallback object2;
    unsigned int expected_cbs = 0;

    mainloop = g_main_loop_new(NULL, FALSE);
        
    dbustl::ObjectProxy pythonObjectProxy(session, "/PythonServerObject", "com.example.SampleService");
    pythonObjectProxy.setInterface("com.example.SampleInterface");

 
#ifdef DBUSTL_CXX0X
    UserMethodCallback object;
    UserMethodCallbackNoReturn noreturnobject;
 
    TRY {
        std::cout << ">0 arg asynchronous call : Functor callback" << std::endl;
        pythonObjectProxy.asyncCall("SimpleProc", UserFunctorCallbackNoReturn());
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )
    
    TRY {
        std::cout << ">0 arg asynchronous call : Method callback on non const object" << std::endl;
        pythonObjectProxy.asyncCall("SimpleProc", &UserMethodCallbackNoReturn::method, &noreturnobject);
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    TRY {
        std::cout << ">0 arg asynchronous call : function callback" << std::endl;
        pythonObjectProxy.asyncCall("SimpleProc", &userFunctionCallbackNoReturn); 
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    TRY {
        std::cout << ">1 arg asynchronous call : Functor callback" << std::endl;
        pythonObjectProxy.asyncCall("SimpleHello", UserFunctorCallback(), "Hi");
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )
    
    TRY {
        std::cout << ">1 arg asynchronous call : Method callback on non const object" << std::endl;
        pythonObjectProxy.asyncCall("SimpleHello", &UserMethodCallback::method, &object, "Hi");
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    TRY {
        std::cout << ">1 arg asynchronous call : function callback" << std::endl;
        pythonObjectProxy.asyncCall("SimpleHello", &userFunctionCallback, "Hi"); 
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    TRY {
        std::cout << ">Interface modifier" << std::endl;
        pythonObjectProxy.setInterface("BOGUS.INTERFACE");
        pythonObjectProxy.asyncCall("SimpleHello", &userFunctionCallback,
          dbustl::ObjectProxy::Interface("com.example.SampleInterface"), "Hi"); 
        pythonObjectProxy.setInterface("com.example.SampleInterface");
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    TRY {
        std::cout << ">Wrong method test" << std::endl;
        pythonObjectProxy.asyncCall("InexistingMethod", &inexistingMethodCallback, 0, 1.0); 
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    TRY {
        std::cout << ">Timeout test" << std::endl;
        pythonObjectProxy.setTimeout(500);
        pythonObjectProxy.asyncCall("test_sleep_2s", &sleepMethodCallback); 
        pythonObjectProxy.setTimeout(-1);
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )
#endif /* DBUSTL_CXX0X */

    TRY {
        std::cout << ">NOVT: 0 arg asynchronous call : Functor callback" << std::endl;
        dbustl::Message callMsg = pythonObjectProxy.createMethodCall("SimpleProc");
        pythonObjectProxy.asyncCall(callMsg, UserFunctorCallbackNoReturn());
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )
    
    TRY {
        std::cout << ">NOVT: 0 arg asynchronous call : function callback" << std::endl;
        dbustl::Message callMsg = pythonObjectProxy.createMethodCall("SimpleProc");
        pythonObjectProxy.asyncCall(callMsg, &userFunctionCallbackNoReturn); 
        expected_cbs++; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    TRY {
        std::cout << ">Signal tests" << std::endl;
        pythonObjectProxy.setSignalHandler("exampleSignal", &exampleSignalCallback); 
        pythonObjectProxy.setSignalHandler("exampleSignal2", &ExampleSignal2MethodCallback::method, &object2); 
        pythonObjectProxy.setSignalHandler("exampleSignal3", ExampleSignal3FunctorCallback()); 
        pythonObjectProxy.setSignalHandler("", ExampleSignalXFunctorCallback()); 
        dbustl::Message callMsg = pythonObjectProxy.createMethodCall("SendSignals");
        pythonObjectProxy.asyncCall(callMsg, &voidMethodCallback);
        //We expect 4 signals to be received
        expected_cbs += 4; 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    //This last call is just to make sure that the mainloop finishes at some time
    TRY {
        dbustl::Message callMsg = pythonObjectProxy.createMethodCall("SimpleProc");
        pythonObjectProxy.asyncCall(callMsg, &stopMethodCallback); 
    }
    CATCH(const std::exception& e,
        std::cerr << e.what() << std::endl;
        return 1;
    )

    g_main_loop_run(mainloop);
    g_main_loop_unref(mainloop);
    
    assert(n_cbs == expected_cbs);
    
    return 0;
}

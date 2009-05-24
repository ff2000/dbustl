/*
 *  DBusTL - D-Bus Template Library
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
#include <cstdlib>
#include <stdexcept>

GMainLoop *mainloop;

class TestServiceClass : private dbustl::DBusObject {
public:
    TestServiceClass(dbustl::Connection *conn) : DBusObject("/ServerObject", "com.example.SampleInterface", conn) {
        exportMethod("test_ref", this, &TestServiceClass::test_ref);
        exportMethod("test_const", this, &TestServiceClass::test_const);
        exportMethod("test_const_ref", this, &TestServiceClass::test_const_ref);
        exportMethod("test_ref_2", this, &TestServiceClass::test_ref_2);
        exportMethod("test_const_2", this, &TestServiceClass::test_const_2);
        exportMethod("test_const_ref_2", this, &TestServiceClass::test_const_ref_2);

        exportMethod("test_callvoid0", this, &TestServiceClass::test_callvoid0);
        exportMethod("test_call0", this, &TestServiceClass::test_call0);        
        exportMethod("test_callvoid1", this, &TestServiceClass::test_callvoid1);
        exportMethod("test_call1", this, &TestServiceClass::test_call1);        
        exportMethod("test_callvoid2", this, &TestServiceClass::test_callvoid2);
        exportMethod("test_call2", this, &TestServiceClass::test_call2);        
        exportMethod("test_callvoid3", this, &TestServiceClass::test_callvoid3);
        exportMethod("test_call3", this, &TestServiceClass::test_call3);        

        exportMethod("test_flexible_executor", this, &TestServiceClass::test_flexible_executor);        

        exportMethod("test_ex1", this, &TestServiceClass::test_ex1);        
        exportMethod("test_ex2", this, &TestServiceClass::test_ex2);        
        exportMethod("test_ex3", this, &TestServiceClass::test_ex3);        

        exportMethod("test_if", this, &TestServiceClass::test_if1, "com.example.Interface1");        
        exportMethod("test_if", this, &TestServiceClass::test_if2, "com.example.Interface2");        

        exportMethod("test_signal", this, &TestServiceClass::test_signal);        
        exportMethod("test_signal2", this, &TestServiceClass::test_signal2);        
        exportMethod("test_signal3", this, &TestServiceClass::test_signal3);        

        exportMethod("stop", this, &TestServiceClass::stop);
        
        exportSignal<std::string>("TestSignal");
        exportSignal<std::string, int>("TestSignal2");
        exportSignal<std::string>("TestSignal3", "com.example.AlternateInterface");

        exportSignal("TestExportSignal0");
        exportSignal<std::string>("TestExportSignal1");
        exportSignal<std::string, std::string>("TestExportSignal2");
        exportSignal<std::string, std::string, std::string>("TestExportSignal3");
        exportSignal<std::string, std::string, std::string, std::string>("TestExportSignal4");
        exportSignal<std::string, std::string, std::string, std::string, std::string>("TestExportSignal5");
        exportSignal<std::string, std::string, std::string, std::string, std::string, std::string>("TestExportSignal6");
        exportSignal<std::string, std::string, std::string, std::string, std::string, std::string, std::string>("TestExportSignal7");
        exportSignal<std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string>("TestExportSignal8");
        exportSignal<std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string>("TestExportSignal9");
        exportSignal<std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string, std::string>("TestExportSignal10");

        exportSignal("WrongSignatureSignal");
    }

private:
    void test_ref(std::string&) {};
    void test_const(const std::string) {};
    void test_const_ref(const std::string&) {};

    std::string& test_ref_2(std::string&) { static std::string s; return s; };
    const std::string test_const_2(const std::string) { return ""; };
    const std::string& test_const_ref_2(const std::string&) { static std::string s; return s; };
    
    void test_callvoid0(void) 
    {
        std::cerr << __FUNCTION__<< std::endl;
    };
    int test_call0(void)
    {
        std::cerr << __FUNCTION__<< std::endl;
        return 0;
    };

    void test_callvoid1(int p1)
    {
        std::cerr << __FUNCTION__<< ":" << p1 << std::endl;
    };
    int test_call1(int p1)
    {
        std::cerr << __FUNCTION__<< std::endl;
        return p1;
    }

    void test_callvoid2(int p1, int p2)
    {
        std::cerr << __FUNCTION__<< ":" << p1 + p2 << std::endl;
    };
    int test_call2(int p1, int p2)
    {
        std::cerr << __FUNCTION__<< std::endl;
        return p1 + p2;
    }

    void test_callvoid3(int p1, int p2, int p3)
    {
        std::cerr << __FUNCTION__<< ":" << p1 + p2 + p3 << std::endl;
    };
    int test_call3(int p1, int p2, int p3)
    {
        std::cerr << __FUNCTION__<< std::endl;
        return p1 + p2 + p3;
    }

    void test_ex1()
    {
      throw dbustl::DBusException("org.mycompany.test", "This is a test exception");
    }

    void test_ex2()
    {
      throw std::runtime_error("Error");
    }

    void test_ex3()
    {
      throw "Test";
    }

    std::string test_if1()
    {
        return "com.example.Interface1";
    }

    std::string test_if2()
    {
        return "com.example.Interface2";
    }

    void test_flexible_executor(dbustl::Message call)
    {
        double a, b;
        call >> a >> b;
        if(!call.error()){
            if(b == 0) {
                dbustl::Message mreturn = call.createErrorReply("org.mycompany.test", "Division by 0");
                sendReply(mreturn);
            }
            else {
                dbustl::Message mreturn = call.createMethodReturn();
                mreturn << (a / b);
                sendReply(mreturn);
            }
        }
    }

    void test_signal()
    {
#ifdef DBUSTL_CXX0X
        emitSignal("TestSignal", "Signal 1 value");
#else
        dbustl::Message signal = createSignal("TestSignal");
        signal << "Signal 1 value";
        emitSignal(signal);
#endif
    };

    void test_signal2()
    {
        dbustl::Message signal = createSignal("TestSignal2");
        signal << "Signal 2 string value" << -1;
        emitSignal(signal);
    };

    void test_signal3()
    {
#ifdef DBUSTL_CXX0X
        emitSignal("TestSignal3", "Signal 3 value", dbustl::Interface("com.example.AlternateInterface"));
#else
        dbustl::Message signal = createSignal("TestSignal3", "com.example.AlternateInterface");
        signal << "Signal 3 value";
        emitSignal(signal);
#endif
    };

    void stop()
    {
        g_main_loop_quit(mainloop);
    };

public:
    void test_unexistingsignal()
    {
        dbustl::Message signal = createSignal("UnexistingSignal");
        emitSignal(signal);
    };

    void test_wrongsignaturesignal()
    {
        dbustl::Message signal = createSignal("WrongSignatureSignal");
        signal << "Value";
        emitSignal(signal);
    };
};

class ChildClass : private dbustl::DBusObject {
public:
    ChildClass(dbustl::Connection *conn) : DBusObject("/ServerObject/Child", "com.example.DontcareInterface", conn) {
        exportSignal<int>("DontcareSignal");
    }
};

class NotChildClass : public dbustl::DBusObject {
public:
    NotChildClass(dbustl::Connection *conn) : DBusObject("/ABogusPath", "com.example.DontcareInterface", conn) {
        exportSignal<int>("DontcareSignal");
    }
};

int main()
{    
    dbustl::GlibEventLoopIntegration mli;
    dbustl::Connection::useEventLoop(mli);    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
        
    assert(session->isConnected());
    
    session->busRequestName("com.example.SampleService");
    
    TestServiceClass srv(session);
    ChildClass child(session);
    NotChildClass notchild(session);
    
    /* Line below tests setPath() method, inclusing a bogus / at the end.
     * Otherwise i would a have set directly the right path */
    notchild.setPath("/Not/A/Child/Class/");
    
    mainloop = g_main_loop_new(NULL, FALSE);
    
    try {
        srv.test_unexistingsignal();
        abort();
    }
    catch(const dbustl::DBusException& e) {
        assert(e.name() == "org.dbustl.UnknownSignal");
        assert(e.message() == "Signal \"UnexistingSignal\" has not been registered with DBusTL");
    }
    
    try {
        srv.test_wrongsignaturesignal();
        abort();
    }
    catch(const dbustl::DBusException& e) {
        assert(e.name() == "org.dbustl.SignalSignatureMismatch");
        assert(e.message() == "Signal \"WrongSignatureSignal\" has been exported with a different signature: '' vs 's'");
    }
    
    g_main_loop_run(mainloop);        
    session->busReleaseName("com.example.SampleService");

    g_main_loop_unref(mainloop);

    return 0;
}

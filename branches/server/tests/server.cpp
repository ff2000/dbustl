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

        exportMethod("test_ex1", this, &TestServiceClass::test_ex1);        
        exportMethod("test_ex2", this, &TestServiceClass::test_ex2);        
        exportMethod("test_ex3", this, &TestServiceClass::test_ex3);        

        exportMethod("test_signal", this, &TestServiceClass::test_signal);        
        exportMethod("test_signal2", this, &TestServiceClass::test_signal2);        

        exportMethod("stop", this, &TestServiceClass::stop);        
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
        signal << "Signal 2 value";
        emitSignal(signal);
    };

    void stop()
    {
        g_main_loop_quit(mainloop);
    };
};

int main()
{    
    dbustl::GlibEventLoopIntegration mli;
    dbustl::Connection::useEventLoop(mli);    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
        
    assert(session->isConnected());
    
    session->busRequestName("com.example.SampleService");
    
    TestServiceClass srv(session);
    
    mainloop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(mainloop);        
    session->busReleaseName("com.example.SampleService");

    g_main_loop_unref(mainloop);

    return 0;
}

/*
 *  DBUSTL - DBus Template Library
 *
 *  Copyright (C) 2008  Fabien Chevalier <fabchevalier@free.fr>
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

#include <iostream>
#include <string>
#include <map>

#include <cassert>

#ifdef DBUSTL_VARIADIC_TEMPLATES
int run_vt_tests()
{
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    bool exception_thrown;
    
    try {
        std::cout << ">Basic procedure call" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        std::cout << ">list of string " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        std::cout << ">set of string " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::set<std::string> in, out;
        in.insert("String 1");
        in.insert("String 2");
        in.insert("String 3");
        pythonServerProxy.call("test_array_of_string", in, &out); 
        assert(in == out);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">list of int " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        std::cout << ">Dict of int->string " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        std::map<int32_t, std::string> in, out;
        in[0] = "Entry 0";
        in[1] = "Entry 1";
        in[2] = "Entry 2";
        pythonServerProxy.call("test_dict_of_integer_string", in, &out); 
        assert(in == out);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    try {
        std::cout << ">list of list of integers" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        std::cout << ">vector of vector of integers" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        std::string message;
        pythonServerProxy.call("SimpleHello", 
          dbustl::ServerProxy::Interface("com.example.SampleInterface"), "Hi", &message); 
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        std::cout << ">Wrong method test" << std::endl;
        exception_thrown = false;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        pythonServerProxy.call("InexistingMethod", "Hi"); 
    }
    catch(const dbustl::DBusException& e) {
        if(e.isSet() && e.name() == std::string("org.freedesktop.DBus.Error.UnknownMethod")) {
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

    try {
        std::cout << ">Wrong return type test" << std::endl;
        exception_thrown = false;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
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
#else
int run_vt_tests()
{
    return 0;
}
#endif /* DBUSTL_VARIADIC_TEMPLATES */


int main()
{    
    int ret;
    ret = run_vt_tests();
    if(ret)
        return ret;
      
    dbustl::Connection *session = dbustl::Connection::sessionBus();

    try {
        std::cout << ">NOVT: bool parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        pythonServerProxy.setInterface("com.example.SampleInterface");
        dbustl::Message callMsg = pythonServerProxy.createMethodCall("test_boolean");
        callMsg << false;
        dbustl::Message callReply = pythonServerProxy.call(callMsg);
        bool value;
        callReply >> value;
        assert(value == true);            
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

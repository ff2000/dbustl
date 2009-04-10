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

#include <iostream>
#include <string>
#include <map>

#include <cassert>

#ifdef DBUSTL_NO_EXCEPTIONS
    #define TRY
    #define CATCH(ex, handler) if(pythonServerProxy.hasError()) { ex = pythonServerProxy.error(); handler }
#else
    #define TRY try
    #define CATCH(ex, handler) catch(ex) {handler}
#endif

struct Struct1 {
    std::vector<std::string> field1;
};
bool operator==(const Struct1& f1, const Struct1& f2){
    return
        (f1.field1 == f2.field1) &&
    true;
};
DBUSTL_REGISTER_STRUCT_1(Struct1, field1);

struct Struct2 {
    std::vector<std::string> field1;
    uint32_t field2;
    Struct2() {field2 = 0;};
};
bool operator==(const Struct2& f1, const Struct2& f2){
    return
        (f1.field1 == f2.field1) &&
        (f1.field2 == f2.field2) &&
    true;
}
DBUSTL_REGISTER_STRUCT_2(Struct2, field1, field2);

struct Struct3 {
    std::vector<std::string> field1;
    uint32_t field2;
    uint32_t field3;
    Struct3() {field2 = 0; field3 = 0;};
};
bool operator==(const Struct3& f1, const Struct3& f2){
    return
        (f1.field1 == f2.field1) &&
        (f1.field2 == f2.field2) &&
        (f1.field3 == f2.field3) &&
    true;
}
DBUSTL_REGISTER_STRUCT_3(Struct3, field1, field2, field3);

struct Struct4 {
    std::vector<std::string> field1;
    uint32_t field2;
    uint32_t field3;
    uint32_t field4;
    Struct4() {field2 = 0; field3 = 0; field4 = 0;};
};
bool operator==(const Struct4& f1, const Struct4& f2){
    return
        (f1.field1 == f2.field1) &&
        (f1.field2 == f2.field2) &&
        (f1.field3 == f2.field3) &&
        (f1.field4 == f2.field4) &&
    true;
}
DBUSTL_REGISTER_STRUCT_4(Struct4, field1, field2, field3, field4);

struct Struct5 {
    std::vector<std::string> field1;
    uint32_t field2;
    uint32_t field3;
    uint32_t field4;
    uint32_t field5;
    Struct5() {field2 = 0; field3 = 0; field4 = 0; field5 = 0;};
};
bool operator==(const Struct5& f1, const Struct5& f2){
    return
        (f1.field1 == f2.field1) &&
        (f1.field2 == f2.field2) &&
        (f1.field3 == f2.field3) &&
        (f1.field4 == f2.field4) &&
        (f1.field5 == f2.field5) &&
    true;
}
DBUSTL_REGISTER_STRUCT_5(Struct5, field1, field2, field3, field4, field5);

struct Struct6 {
    std::vector<std::string> field1;
    uint32_t field2;
    uint32_t field3;
    uint32_t field4;
    uint32_t field5;
    uint32_t field6;
    Struct6() {field2 = 0; field3 = 0; field4 = 0; field5 = 0; field6 = 0;};
};
bool operator==(const Struct6& f1, const Struct6& f2){
    return
        (f1.field1 == f2.field1) &&
        (f1.field2 == f2.field2) &&
        (f1.field3 == f2.field3) &&
        (f1.field4 == f2.field4) &&
        (f1.field5 == f2.field5) &&
        (f1.field6 == f2.field6) &&
    true;
}
DBUSTL_REGISTER_STRUCT_6(Struct6, field1, field2, field3, field4, field5, field6);

#ifdef DBUSTL_VARIADIC_TEMPLATES
int run_vt_tests()
{
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    bool exception_thrown;
    
    {
        std::cout << ">Basic procedure call" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::string message;
            pythonServerProxy.call("SimpleProc"); 
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">String parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::string message;
            pythonServerProxy.call("SimpleHello", std::string("Hi"), &message); 
            assert(message == "Hi");            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">char* parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::string message;
            pythonServerProxy.call("SimpleHello", "Hi", &message); 
            assert(message == "Hi");            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    
    {
        std::cout << ">bool parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            bool value;
            pythonServerProxy.call("test_boolean", false, &value); 
            assert(value == true);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">byte parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            unsigned int in = 254;
            unsigned int out;
            pythonServerProxy.call("test_byte", in, &out); 
            assert(in == (out - 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">uint16 parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            uint16_t in = 65534;
            uint16_t out;
            pythonServerProxy.call("test_uint16", in, &out); 
            assert(in == (out - 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">int16 parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            int16_t in = 0;
            int16_t out;
            pythonServerProxy.call("test_int16", in, &out); 
            assert(in == (out + 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">uint32 parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            uint32_t in = (2^32) - 1;
            uint32_t out;
            pythonServerProxy.call("test_uint32", in, &out); 
            assert(in == (out - 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">int32 parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            int32_t in = 0;
            int32_t out;
            pythonServerProxy.call("test_int32", in, &out); 
            assert(in == (out + 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">uint64 parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            uint64_t in = (2^64) - 1;
            uint64_t out;
            pythonServerProxy.call("test_uint64", in, &out); 
            assert(in == (out - 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">int64 parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");        
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            int64_t in = 0;
            int64_t out;
            pythonServerProxy.call("test_int64", in, &out); 
            assert(in == (out + 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">unsigned long parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            unsigned long in = (2^32) - 1;
            unsigned long out;
            pythonServerProxy.call("test_uint32", in, &out); 
            assert(in == (out - 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">long parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            long in = 0;
            long out;
            pythonServerProxy.call("test_int32", in, &out); 
            assert(in == (out + 1));            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">double parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            double in = 1.11;
            double out;
            pythonServerProxy.call("test_double", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">float promotion to double " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            float in = 1.11;
            double out;
            pythonServerProxy.call("test_double", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">list of string " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::list<std::string> in, out;
            in.push_back("String 1");
            in.push_back("String 2");
            in.push_back("String 3");
            pythonServerProxy.call("test_array_of_string", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">set of string " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::set<std::string> in, out;
            in.insert("String 1");
            in.insert("String 2");
            in.insert("String 3");
            pythonServerProxy.call("test_array_of_string", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">multiset of string " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::multiset<std::string> in, out;
            in.insert("String 1");
            in.insert("String 1");
            in.insert("String 2");
            pythonServerProxy.call("test_array_of_string", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">set collision " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::list<std::string> in;
            std::set<std::string> out;
            in.push_back("String 1");
            in.push_back("String 1");
            in.push_back("String 2");
            pythonServerProxy.call("test_array_of_string", in, &out); 
            assert(out.size() == 2);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">list of int " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::list<int32_t> in, out;
            in.push_back(1);
            in.push_back(2);
            in.push_back(3);
            pythonServerProxy.call("test_array_of_int", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">map of int->string " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::map<int32_t, std::string> in, out;
            in[0] = "EnTRY 0";
            in[1] = "EnTRY 1";
            in[2] = "EnTRY 2";
            pythonServerProxy.call("test_dict_of_integer_string", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">multimap of int->string " << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::map<int32_t, std::string> in;
            std::multimap<int32_t, std::string> out;
            in[0] = "EnTRY 0";
            in[1] = "EnTRY 1";
            in[2] = "EnTRY 2";
            pythonServerProxy.call("test_dict_of_integer_string", in, &out); 
            assert(out.size() == 3);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">list of list of integers" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
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
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">vector of vector of integers" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
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
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    
    {
        std::cout << ">Struct with 1 parameter" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::map<uint32_t, Struct1> in, out;
            Struct1 s;
            s.field1.push_back("String");
            in[0] = s;
            pythonServerProxy.call("test_struct1", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">Struct with 2 parameters" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::map<uint32_t, Struct2> in, out;
            Struct2 s;
            s.field1.push_back("String");
            in[0] = s;
            pythonServerProxy.call("test_struct2", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">Struct with 3 parameters" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::map<uint32_t, Struct3> in, out;
            Struct3 s;
            s.field1.push_back("String");
            in[0] = s;
            pythonServerProxy.call("test_struct3", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">Struct with 4 parameters" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::map<uint32_t, Struct4> in, out;
            Struct4 s;
            s.field1.push_back("String");
            in[0] = s;
            pythonServerProxy.call("test_struct4", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">Struct with 5 parameters" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::map<uint32_t, Struct5> in, out;
            Struct5 s;
            s.field1.push_back("String");
            in[0] = s;
            pythonServerProxy.call("test_struct5", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">Struct with 6 parameters" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            std::map<uint32_t, Struct6> in, out;
            Struct6 s;
            s.field1.push_back("String");
            in[0] = s;
            pythonServerProxy.call("test_struct6", in, &out); 
            assert(in == out);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">Interface modifier" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            std::string message;
            pythonServerProxy.call("SimpleHello", 
              dbustl::ServerProxy::Interface("com.example.SampleInterface"), "Hi", &message); 
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }

    {
        std::cout << ">Wrong method test" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            exception_thrown = false;
            pythonServerProxy.setInterface("com.example.SampleInterface");
            pythonServerProxy.call("InexistingMethod", "Hi"); 
        }
        CATCH(const dbustl::DBusException& e,
            if(e.isSet() && e.name() == std::string("org.freedesktop.DBus.Error.UnknownMethod")) {
                exception_thrown = true;
            }
            else {
                std::cerr << e.what() << std::endl;
                return 1;
            }
        )
        if(!exception_thrown) {
            std::cerr << "No exception thrown !!" << std::endl;
            return 1;
        }
    }

    {
        std::cout << ">Wrong return type test" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            exception_thrown = false;
            pythonServerProxy.setInterface("com.example.SampleInterface");
            int message;
            pythonServerProxy.call("SimpleHello", "Hi", &message); 
        }
        CATCH(const std::exception& e,
            if(e.what() == std::string("org.dbustl.MethodReplyError: Unable to assign D-Bus value with signature 's' to argument at position (0)")) {
                exception_thrown = true;
            }
            else {
                std::cerr << e.what() << std::endl;
                return 1;
            }
        )
        if(!exception_thrown) {
            std::cerr << "No exception thrown !!" << std::endl;
            return 1;
        }
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

    {
        std::cout << ">NOVT: bool parameter passing" << std::endl;
        dbustl::ServerProxy pythonServerProxy(session, "/PythonServerObject", "com.example.SampleService");
        TRY {
            pythonServerProxy.setInterface("com.example.SampleInterface");
            dbustl::Message callMsg = pythonServerProxy.createMethodCall("test_boolean");
            callMsg << false;
            dbustl::Message callReply = pythonServerProxy.call(callMsg);
            bool value;
            callReply >> value;
            assert(value == true);            
        }
        CATCH(const std::exception& e,
            std::cerr << e.what() << std::endl;
            return 1;
        )
    }
    return 0;
}

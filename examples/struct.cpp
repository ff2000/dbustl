/**
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

#include "struct.h"
#include <string>
#include <iostream>

using namespace std;
using namespace dbustl;
 
int main()
{
    ObjectProxy remoteObject(Connection::sessionBus(), 
    "/PythonServerObject", "com.example.SampleService");
    try {
        ExampleStruct s1, s2;
        s1.field1 = "String";
        s1.field2 = 1.1;
        remoteObject.call("test_ExampleStruct", s1, &s2); 
        cout << s1.field1 << ", " << s1.field2 << endl;
    }
    catch(const DBusException& e) {
        /* Dbus call failed: e.name() constains error cause */
        cerr << e.what() << endl;
    } 
    return 0;
}

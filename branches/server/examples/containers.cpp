/** @cond
 * 
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
 * @endcond
 */
 
#include <dbustl-1/dbustl>

#include <string>

using namespace std;
using namespace dbustl;
 
int main()
{
    ObjectProxy remoteObject(Connection::sessionBus(), 
        "/PythonServerObject", "com.example.SampleService");
    try {
        list<int32_t> in, out;
        in.push_back(1);
        in.push_back(2);
        in.push_back(3);
        remoteObject.call("test_array_of_int", in, &out); 

        if(in == out)
            cout << "Received a valid reply" << endl;
    }
    catch(const DBusException& e) {
        /* Dbus call failed: e.name() constains error cause */
        cerr << e.what() << endl;
    } 
    return 0;
}

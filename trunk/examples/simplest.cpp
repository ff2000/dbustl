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

#include <string>

using namespace std;
using namespace dbustl;
 
int main()
{
    cout << "Saying hello to /PythonServerObject" << endl;
    try {
        ServerProxy remoteObject(Connection::sessionBus(), "/PythonServerObject", "com.example.SampleService");
        string reply;
        remoteObject.call("SimpleHello", "Hi", &reply); 
        cout << "Reply: " << reply << endl;
	}
	catch(const dbustl::DBusException& e) {
		cerr << "Method call error " << e.what();
		return 1;
	}
 	
 	return 0;
}

/**
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
    ServerProxy remoteObject(Connection::sessionBus(), 
        "/PythonServerObject", "com.example.SampleService");
    try {
        Message methodMsg(NULL), replyMsg(NULL);
        string reply;
        
        /* Here we create the method call message, and adds all the 
         * parametersusing the << operator 
         */
        methodMsg = remoteObject.createMethodCall("SimpleHello");
        methodMsg << "Hello world";
        /* We then send the method call message and wait for the reply */
        replyMsg = remoteObject.call(methodMsg); 
        /* Deserialization of the arguments in the reply message */
        replyMsg >> reply;
        /* isValid() checks that reading reply message parameters suceeded */
        if(replyMsg.isValid()) {
            /* reply now contains a valid string */
            cout << reply << endl;
        }
    }
    catch(const DBusException& e) {
        /* Dbus call failed: e.name() constains error cause */
        cerr << e.what() << endl;
    } 
    return 0;
}

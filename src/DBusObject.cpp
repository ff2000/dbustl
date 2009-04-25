/*
 *  DBusTL - DBus Template Library
 *
 *  Copyright (C) 2008, 2009  Fabien Chevalier <chefabien@gmail.com>
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

#include <dbus/dbus.h>

#include <dbustl-1/DBusObject>
#include <dbustl-1/Connection>
#include <dbustl-1/Message>

#include <cassert>

namespace dbustl {

DBusObjectPathVTable DBusObject::_vtable = {
    NULL,
    &DBusObject::incomingMessagesProcessing, 
    0,
    0,
    0,
    0
};

DBusObject::DBusObject(const std::string& objectPath, const std::string& interface, Connection *conn) 
 : _conn(0), _objectPath(objectPath), _interface(interface)
{
    if(conn) {
        enable(conn);
    }
}
            
void DBusObject::enable(Connection * conn)
{
    assert(conn->isConnected());
    errorReset();
    if(_conn) {
        disable();
    }

    DBusException ex;
    if(dbus_connection_try_register_object_path(conn->dbus(), _objectPath.c_str(), &_vtable, this, ex.dbus())) {
        _conn = conn;
    }
    else {
        throw_or_set(ex);
    }
}

void DBusObject::disable()
{
    errorReset();
    dbus_connection_unregister_object_path(_conn->dbus(), _objectPath.c_str());
    _conn = 0;
}

void DBusObject::exportMethod(const std::string& methodName, MethodExecutorBase *executor)
{
    std::pair<MethodContainerType::iterator, MethodContainerType::iterator> its(_exportedMethods.equal_range(methodName));
    MethodContainerType::iterator firstMatch = its.first;
    MethodContainerType::iterator lastMatch = its.second;
    if(executor->interface().empty()) {
        executor->setInterface(_interface);
    }
    for(; (firstMatch != lastMatch) && (firstMatch->second->interface() != executor->interface()); ++firstMatch) {};
    if(firstMatch != lastMatch) {
        //Match found
        MethodExecutorBase* match = firstMatch->second;
        _exportedMethods.erase(firstMatch);        
        delete match;
    }
    _exportedMethods.insert(std::make_pair(methodName, executor));
}

DBusHandlerResult DBusObject::incomingMessagesProcessing(DBusConnection *, 
    DBusMessage *dbusMessage, void *user_data)
{   
    if(dbus_message_get_type(dbusMessage) == DBUS_MESSAGE_TYPE_METHOD_CALL) {
        /* libdbus keeps ownership of the message, but our Message class
         * wants ownership too: as a result both will free the message
         * once it is not used anymore. Ref it one more time as a workaround. */
        dbus_message_ref(dbusMessage);
        
        Message call(dbusMessage);
        std::string methodName = call.member();
        std::string interface = call.interface();
        DBusObject* object = static_cast<DBusObject *>(user_data);

        std::pair<MethodContainerType::iterator, MethodContainerType::iterator> its(object->_exportedMethods.equal_range(methodName));
        MethodContainerType::iterator firstMatch = its.first;
        MethodContainerType::iterator lastMatch = its.second;
        
        MethodExecutorBase* executor = 0;
        
        if(interface.empty()) {
            /* Look for first match */
            if(firstMatch != lastMatch) {
                executor = firstMatch->second;
            }
        }
        else {
            /* Look for exact match */
            for(; (firstMatch != lastMatch) && firstMatch->second->interface() != interface; ++firstMatch) {};
            if(firstMatch != lastMatch) {
                //Match found
                executor = firstMatch->second;
            }
        }
            
        if(executor) {
            Message reply(dbus_message_new_method_return(call.dbus()));
            if(reply.dbus()) {
                try {
                    executor->processCall(&call, &reply);
                    dbus_connection_send(object->_conn->dbus(), reply.dbus(), NULL);
                }
            #ifndef DBUSTL_NO_EXCEPTIONS
                catch(const DBusException& e) {
                    Message errorReply(dbus_message_new_error(call.dbus(), e.name().c_str(), e.message().c_str()));
                    if(errorReply.dbus()) {
                        dbus_connection_send(object->_conn->dbus(), errorReply.dbus(), NULL);
                    }
                }
                catch(const std::exception& e) {
                    Message errorReply(dbus_message_new_error(call.dbus(), "org.dbustl.CPPException", e.what()));
                    if(errorReply.dbus()) {
                        dbus_connection_send(object->_conn->dbus(), errorReply.dbus(), NULL);
                    }
                }
                catch(...) {
                    Message errorReply(dbus_message_new_error(call.dbus(), "org.dbustl.CPPException", "Unknown C++ exception"));
                    if(errorReply.dbus()) {
                        dbus_connection_send(object->_conn->dbus(), errorReply.dbus(), NULL);
                    }
                }
            #endif
            }
      	    return DBUS_HANDLER_RESULT_HANDLED;
        }
    }
  	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

Message DBusObject::createSignal(const std::string& signalName, const std::string& interface)
{
    const char *intf = 
        (interface.empty() ?
            (_interface.empty() ? 
                NULL :
                _interface.c_str()
            ) :
            interface.c_str()
        );
    
    Message signal(dbus_message_new_signal(_objectPath.c_str(), intf, signalName.c_str()));
                    
    if(signal.dbus()) {
        // Blank out error status
        errorReset();
    }
    else {
        throw_or_set(DBUS_ERROR_NO_MEMORY, "Not enough memory to allocate DBUS message");
    }

    return signal;
}

void DBusObject::emitSignal(Message& signal)
{
    if(!signal.error()) {
        dbus_connection_send(_conn->dbus(), signal.dbus(), NULL);
    }
    else {
        throw_or_set(*signal.error());
    }
}

}

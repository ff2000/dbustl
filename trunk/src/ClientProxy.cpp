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

#include <dbus/dbus.h>

#include <dbustl-1/Connection>
#include <dbustl-1/ClientProxy>

namespace dbustl {

DBusObjectPathVTable ClientProxy::_vtable = {
    NULL,
    &ClientProxy::signalsProcessingMethod, 
    0,
    0,
    0,
    0
};

ClientProxy::ClientProxy(Connection* conn, const std::string& path, const std::string& destination) :
  _conn(conn), _path(path), _destination(destination), _timeout(-1)
{
    if(!dbus_connection_register_object_path(_conn->dbus(), _path.c_str(), &_vtable, this)) {
        throw_or_set(DBUS_ERROR_NO_MEMORY, "Not enough memory to register object path handler");
    }
}

ClientProxy::~ClientProxy()
{
    dbus_connection_unregister_object_path(_conn->dbus(), _path.c_str());

    std::map<std::string, FunctorWrapper>::iterator it;
    while((it = _signalsHandlers.begin()) != _signalsHandlers.end()) {
        // Beware : due to the fact the method below erases() the map element while we still
        // need some bits of its (i.e. signal name), we have to make a copy
        std::string sigName = it->first;
        removeSignalHandler(sigName);
    }
}

Message ClientProxy::createMethodCall(const std::string& methodName) const
{
    const char *dest = _destination.empty() ? NULL : _destination.c_str();
    const char *intf = _interface.empty() ? NULL : _interface.c_str();
    
    return dbus_message_new_method_call(dest, _path.c_str(), intf, methodName.c_str());
}

Message ClientProxy::call(Message& method_call)
{
    DBusException error;
    DBusMessage *reply;
          
    reply = dbus_connection_send_with_reply_and_block(_conn->dbus(), method_call.dbus(), _timeout, error.dbus());
    if(!reply) {
        throw_or_set(error);
    }
    return Message(reply);
}

void ClientProxy::processInArgs(Message& msg)
{
    if(msg.isValid()) {
        Message reply(call(msg));
        if(!reply.isNull()) {
            //Overwrite message 
            msg = reply;
        }
    }
    else {
        throw_or_set(DBUS_ERROR_NO_MEMORY, "Not enough memory to send DBUS message");
    }
}

void ClientProxy::processOutArgs(Message&)
{
    //TODO handle case where:
    // Not enough return arguments where provided
    // Too much return arguments are provided
}

DBusHandlerResult ClientProxy::signalsProcessingMethod(DBusConnection *, 
    DBusMessage *dbusMessage, void *user_data)
{
    ClientProxy* proxy = static_cast<ClientProxy *>(user_data);
    
    /* Basic sanity check : libdbus tries an approximate match on objects
     * paths, but we want a perfect match */
    
    if(proxy->_path != dbus_message_get_path(dbusMessage)) {
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
    
    Message msg(dbusMessage);

    /* libdbus keeps ownership of the message, but our Message class
     * wants ownership too: as a result both will free the message
     * once it is not used anymore. Ref it one more time as a workaround. */
    dbus_message_ref(msg.dbus());

    if(dbus_message_get_type(msg.dbus()) == DBUS_MESSAGE_TYPE_SIGNAL) {
        std::string sigName = msg.member();
        std::string handlerName;
        
        //First check if we can have an exact match
        if(proxy->_signalsHandlers.count(sigName)) {
            handlerName = sigName;
        }
        //Second check for a generic handler
        else if(proxy->_signalsHandlers.count("")) {
            handlerName = "";
        }
        else {
            return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
        }
        
        FunctorWrapper& fw = proxy->_signalsHandlers[handlerName];
        
        (*fw.call_functor)(fw.functor, msg);

    	return DBUS_HANDLER_RESULT_HANDLED;
    }
    else {
    	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
}

bool ClientProxy::removeSignalHandler(const std::string& signalName)
{
    if(_signalsHandlers.count(signalName)) {
        _signalsHandlers[signalName].delete_functor(_signalsHandlers[signalName].functor);
        _signalsHandlers.erase(signalName);
        return unWatchSignal(signalName);
    }
    return true;
}

bool ClientProxy::watchSignal(const std::string& signalName)
{
    if(!_conn->isPrivate()) {
        std::stringstream match;
        DBusException error;
        
        match << "type='signal',path='" << _path << "'";
        
        if(signalName.size()) {
            match << ",member='" << signalName << "'";
        }
        
        dbus_bus_add_match(_conn->dbus(), match.str().c_str(), error.dbus());
        if(error.isSet()) {
            throw_or_set(error);
            return false;
        }
    }
    return true;
}

bool ClientProxy::unWatchSignal(const std::string& signalName)
{
    if(!_conn->isPrivate()) {
        std::stringstream match;
        DBusException error;
        
        match << "type='signal',path='" << _path << "'";
        
        if(signalName.size()) {
            match << ",member='" << signalName << "'";
        }
        
        dbus_bus_remove_match(_conn->dbus(), match.str().c_str(), error.dbus());
        if(error.isSet()) {
            throw_or_set(error);
            return false;
        }
    }
    return true;
}

}

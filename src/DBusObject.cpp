/*
 *  DBusTL - D-Bus Template Library
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
#include <dbustl-1/types/Basic> //Required for std::string serialization

#include <set>

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

std::set<const DBusObject*> DBusObject::_objects;

DBusObject::DBusObject(const std::string& objectPath, const std::string& interface, Connection *conn) 
 : _conn(0), _interface(interface)
{
    // We call setPath() here instead of a direct assignation because setPath() performs
    // a trailing slash check
    setPath(objectPath);
    exportMethod("Introspect", this, &DBusObject::introspect, DBUS_INTERFACE_INTROSPECTABLE);
    if(conn) {
        enable(conn);
    }

    _objects.insert(this);
}

DBusObject::~DBusObject() 
{
    _objects.erase(this);

    disable();

    MethodContainerType::iterator it;
    while((it = _exportedMethods.begin()) != _exportedMethods.end()) {
        MethodExecutorBase* match = it->second;
        _exportedMethods.erase(it);
        delete match;
    }
}

void DBusObject::setPath(const std::string& newPath)
{
    assert(!newPath.empty());
    Connection *conn = _conn;
    disable();
    if(newPath[newPath.size() - 1] != '/') {
        _objectPath = newPath;
    }
    else {
        _objectPath = newPath.substr(0, newPath.size() - 1);
    }
    if(conn) {
        enable(conn);
    }
}
            
void DBusObject::enable(Connection * conn)
{
    assert(conn && conn->isConnected());
    errorReset();
    disable();

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
    if(_conn) {
        errorReset();
        dbus_connection_unregister_object_path(_conn->dbus(), _objectPath.c_str());
        _conn = 0;
    }
}

void DBusObject::exportMethodInternal(const std::string& methodName, MethodExecutorBase *executor)
{
    MethodContainerType::iterator firstMatch = _exportedMethods.lower_bound(methodName);
    MethodContainerType::iterator lastMatch = _exportedMethods.upper_bound(methodName);
    
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

        MethodContainerType::iterator firstMatch = object->_exportedMethods.lower_bound(methodName);
        MethodContainerType::iterator lastMatch = object->_exportedMethods.upper_bound(methodName);
        
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
        #ifndef DBUSTL_NO_EXCEPTIONS
            try {
        #endif
                executor->processCall(object, &call);
        #ifndef DBUSTL_NO_EXCEPTIONS
            }
            catch(const DBusException& e) {
                Message errorReply = 
                    call.createErrorReply(e.name(), e.message());
                if(errorReply.dbus()) {
                    object->sendReply(errorReply);
                }
            }
            catch(const std::exception& e) {
                Message errorReply = 
                    call.createErrorReply("org.dbustl.CPPException", e.what());
                if(errorReply.dbus()) {
                    object->sendReply(errorReply);
                }
            }
            catch(...) {
                Message errorReply = 
                    call.createErrorReply("org.dbustl.CPPException", "Unknown C++ exception");
                if(errorReply.dbus()) {
                    object->sendReply(errorReply);
                }
            }
        #endif
      	    //Call is in error state is there was a signature mismatch somewhere
      	    if(!call.error()) {
      	        return DBUS_HANDLER_RESULT_HANDLED;
            }
            else {
      	        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
            }
        }
    }
  	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

void DBusObject::sendReply(Message& reply)
{
    assert(
        (dbus_message_get_type(reply.dbus()) == DBUS_MESSAGE_TYPE_METHOD_RETURN) ||
        (dbus_message_get_type(reply.dbus()) == DBUS_MESSAGE_TYPE_ERROR)
        );
    dbus_connection_send(_conn->dbus(), reply.dbus(), NULL);
}

void DBusObject::exportSignal(const std::string& name, 
    const char* const * signature, const std::string& interface)
{
    ExportedSignal sig((interface.empty() ? _interface : interface), signature);
        
    _exportedSignals.insert(std::make_pair(
        name, 
        sig
    ));
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
        const std::string intf = signal.interface();
        //Do signature sanity check
        bool match_found = false;
        ExportedSignalType::const_iterator begin, end, cur;
        begin = _exportedSignals.lower_bound(signal.member());
        end   = _exportedSignals.upper_bound(signal.member());
        for(cur = begin; cur != end; ++cur) {
            const ExportedSignal& signalInfo = cur->second;
            if(signalInfo.interface() == intf) {
                if(convertSignature(signalInfo.signatures()) == dbus_message_get_signature(signal.dbus())) {
                    match_found = true;
                }
                else {
                    std::string msg = std::string("Signal \"") + signal.member()
                        + "\" has been exported with a different signature: '" 
                        + convertSignature(signalInfo.signatures())
                        + "' vs '" + dbus_message_get_signature(signal.dbus()) + "'";
                    throw_or_set("org.dbustl.SignalSignatureMismatch", msg.c_str());
                    return;
                }
            }
        }
    
        if(match_found) {    
            dbus_connection_send(_conn->dbus(), signal.dbus(), NULL);
        }
        else {
            std::string msg = std::string("Signal \"") + signal.member() + 
                "\" has not been registered with DBusTL";
            throw_or_set("org.dbustl.UnknownSignal", msg.c_str());
        }
    }
    else {
        throw_or_set(*signal.error());
    }
}

std::string DBusObject::introspect()
{
    std::string xmlIntrospect = DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE;
    xmlIntrospect += "<node name=\"" + _objectPath + "\">";

    MethodContainerType::iterator methodsIt;
    //First lookup all available interfaces
    std::set<std::string> interfaces;
    for(methodsIt = _exportedMethods.begin(); methodsIt != _exportedMethods.end(); ++methodsIt) {
        interfaces.insert(methodsIt->second->interface());
    }
    ExportedSignalType::iterator signalsIt;
    for(signalsIt = _exportedSignals.begin(); signalsIt != _exportedSignals.end(); ++signalsIt) {
        interfaces.insert(signalsIt->second.interface());
    }
    
    std::set<std::string>::const_iterator interfacesIt;
    for(interfacesIt = interfaces.begin(); interfacesIt != interfaces.end(); ++interfacesIt) {
        const std::string curInterface = *interfacesIt;
        xmlIntrospect += "<interface name=\"" + curInterface + "\">";
        for(MethodContainerType::const_iterator it = _exportedMethods.begin(); 
                it != _exportedMethods.end(); ++it) {
            MethodExecutorBase *method = it->second;
            if(method->interface() == curInterface) {
                int i;
                const char* const *signatures = method->inSignatures();
                xmlIntrospect += "<method name=\"" + it->first + "\">";
                for(i = 0; signatures[i]; ++i) {
                    xmlIntrospect += argumentIntrospect(signatures[i], DirIn);
                }
                signatures = method->outSignatures();
                for(i = 0; signatures[i]; ++i) {
                    xmlIntrospect += argumentIntrospect(signatures[i], DirOut);
                }
                xmlIntrospect += "</method>";
            }
        }
        for(ExportedSignalType::const_iterator it = _exportedSignals.begin(); 
                it != _exportedSignals.end(); ++it) {
            const ExportedSignal& signal = it->second;
            if(signal.interface() == curInterface) {
                int i = 0;
                const char* const *signatures = signal.signatures();
                xmlIntrospect += "<signal name=\"" + it->first + "\">";
                while(signatures[i]) {
                    xmlIntrospect += argumentIntrospect(signatures[i], DirNone);
                    ++i;
                }
                xmlIntrospect += "</signal>";
            }
        }
        xmlIntrospect += "</interface>"; 
    }
    
    xmlIntrospect += introspectChildren();
    xmlIntrospect += "</node>\n";
    return xmlIntrospect;
}

std::string DBusObject::argumentIntrospect(const char *sig, Direction dir)
{
    static std::string dirs[] = {"", " direction=\"in\"", " direction=\"out\""};
    std::string xml = "<arg type=\"" + std::string(sig) + "\"" + dirs[dir] + "/>";
    return xml;
}

std::string DBusObject::introspectChildren()
{
    std::string xmlIntrospect;
    std::set<const DBusObject*>::iterator it;
    for(it = _objects.begin(); it != _objects.end(); ++it) {
        //check for connection match
        if(_conn == (*it)->_conn) {
            //Check for substring match
            const std::string& childPath = (*it)->_objectPath;
            if(_objectPath.size() < childPath.size() && _objectPath == childPath.substr(0, _objectPath.size())) {
                //Compute relative child name
                std::string name = childPath.substr(_objectPath.size());
                if(name[0] == '/') {
                    name = name.substr(1);
                }
                xmlIntrospect += "<node name=\"" + name + "\"/>";
            }
        }
    }
    return xmlIntrospect;
}

std::string DBusObject::convertSignature(const char *const *sig)
{
    int i = 0;
    std::string s;
    while(sig[i]) {
        s += sig[i];
        ++i;
    }
    return s;
}

}

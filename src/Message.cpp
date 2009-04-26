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

#include <dbustl-1/Message>
#include <dbustl-1/types/Basic> //Needed for operator<< in this file
#include <dbustl-1/DBusException>

#include <sstream>

#include <cassert>

namespace dbustl {

Message::Message(DBusMessage *msg)
  : _msg(msg), _serExcept(0), _iteratorInitialized(false), _parsedArguments(0)
{
}

Message::Message(const Message& other) 
{
    if(other._msg) {
        dbus_message_ref(other._msg);
    }
    _msg = other._msg;
    _serExcept = other._serExcept;
    _iteratorInitialized = other._iteratorInitialized;
    _parsedArguments = other._parsedArguments;
}

Message::~Message()
{
    if(_msg) {
        dbus_message_unref(_msg);
    }
    delete _serExcept;
}

Message& Message::operator=(const Message& other) {
    if(other._msg) {
        dbus_message_ref(other._msg);
    }
    
    if(_msg) {
        dbus_message_unref(_msg);
    }
    
    if(other._serExcept ) {
        _serExcept = new DBusException(*other._serExcept);
    }
    else {
        _serExcept = 0;
    }
    _iteratorInitialized = other._iteratorInitialized;
    _parsedArguments = other._parsedArguments;
    _msg = other._msg;
    return *this;
}

Message& Message::operator=(DBusMessage *msg) {
    if(_msg) {
        dbus_message_unref(_msg);
    }
    
    _serExcept = 0;
    _iteratorInitialized = false;
    _parsedArguments = 0;
    _msg = msg;
    return *this;
}

std::string Message::member() const
{
    const char * mem = NULL;
    if(_msg) {
        mem = dbus_message_get_member(_msg);
    }
    return mem != NULL ? mem : "";
}

std::string Message::interface() const
{
    const char * intf = NULL;
    if(_msg) {
        intf = dbus_message_get_interface(_msg);
    }
    return intf != NULL ? intf : "";
}

Message Message::createMethodReturn() const
{
    assert(_msg);
    assert(dbus_message_get_type(_msg) == DBUS_MESSAGE_TYPE_METHOD_CALL);
    return Message(dbus_message_new_method_return(_msg));
}

Message Message::createErrorMessage(const std::string& name, const std::string& message) const
{
    const char *msg = message.empty() ? NULL: message.c_str();
    assert(_msg);
    assert(dbus_message_get_type(_msg) == DBUS_MESSAGE_TYPE_METHOD_CALL);
    return Message(dbus_message_new_error(_msg, name.c_str(), msg));
}

bool Message::serializationInit()
{
    if(!_msg) {
        return false;
    }
    
    if(!_iteratorInitialized) {
        dbus_message_iter_init_append(_msg, &_it);
        _iteratorInitialized = true;
    }
    
    //If message is already screwed up, we just discard the other arguments
    return !_serExcept;
}

bool Message::deSerializationInit()
{
    if(!_msg || _serExcept) {
        return false;
    }

    if(_iteratorInitialized) {
        dbus_message_iter_next(&_it);
    }
    else {
        dbus_message_iter_init(_msg, &_it);
        _iteratorInitialized = true;
    }

    //If we are past the end, we mark the message as invalid
    if(dbus_message_iter_get_arg_type(&_it) == DBUS_TYPE_INVALID) {
        _serExcept = new DBusException("org.dbustl.MethodReplyError", 
            "All arguments in D-Bus method reply message have already been consumed");
    }

    return !_serExcept;
}

void Message::setSerializationError()
{
    std::stringstream ss;
    ss << "Unable to serialize parameter at index " << _parsedArguments;
    _serExcept = new DBusException("org.dbustl.MethodCallError", ss.str().c_str());
}

void Message::setDeserializationError()
{
    std::stringstream ss;
    char * sign = dbus_message_iter_get_signature(&_it);
    ss << "Unable to assign D-Bus value with signature '" << sign 
       << "' to argument at position (" << _parsedArguments << ")";
    dbus_free(sign);
    _serExcept = new DBusException("org.dbustl.MethodReplyError", ss.str().c_str());
}

Message& Message::operator<<(const char* inarg)
{
    using namespace types;

    if(serializationInit()) {
        // Note: if we are here we cannot already have an exception
        if(Serializer<const char *>::run(&_it, inarg) == FALSE) {
            setSerializationError();
        }
    }

    return *this;
}

}

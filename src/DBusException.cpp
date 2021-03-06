/*
 *  DBusTL - D-Bus Template Library
 *
 *  Copyright (C) 2008, 2009  Fabien Chevalier <chefabien@gmail.com>
 *  
 *
 *  This file is part of the D-Bus Template Library.
 *
 *  The D-Bus Template Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  D-Bus Template Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with D-Bus Template Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <cassert>
#include <dbustl-1/DBusException>

namespace dbustl {

DBusException::DBusException()
{
    dbus_error_init(&_dbusError);
}

DBusException::DBusException(const std::string& name, const std::string& message)
{
    dbus_error_init(&_dbusError);
    dbus_set_error(&_dbusError, name.c_str(), "%s", message.c_str());
}

//Copy constructor: special care must be taken
DBusException::DBusException(const DBusException& e)
{
    dbus_error_init(&_dbusError);
    dbus_set_error(&_dbusError, e._dbusError.name, "%s", e._dbusError.message);
}

//assignmen operator: special care must be taken
DBusException& DBusException::operator=(const DBusException& e)
{
    dbus_error_free(&_dbusError);
    if(dbus_error_is_set(&e._dbusError)) {
      dbus_set_error(&_dbusError, e._dbusError.name, "%s", e._dbusError.message);
    }
    return *this;
}

DBusException::~DBusException() throw()
{
    dbus_error_free(&_dbusError);
}

//Expanded description of the exception of the form "name: description"
const char* DBusException::what() const throw()
{
    if(_whatMsg.empty()) {
        _whatMsg = name() + ": " + message();
    }
    return _whatMsg.c_str();
}

std::string DBusException::name() const
{
    return _dbusError.name;
}

//A more verbose description of the exception
std::string DBusException::message() const
{
    return _dbusError.message;
}
  
bool DBusException::isSet() const
{
    return dbus_error_is_set(&_dbusError) == TRUE;
}

}

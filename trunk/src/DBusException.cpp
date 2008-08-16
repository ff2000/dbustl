#include <cassert>
#include <dbustl/DBusException>

namespace dbustl {

DBusException::DBusException() throw()
{
    dbus_error_init(&_dbusError);
}

DBusException::DBusException(const std::string& name, const std::string& message) throw()
{
    dbus_error_init(&_dbusError);
    dbus_set_error(&_dbusError, name.c_str(), "%s", message.c_str());
}

//Copy constructor: special care must be taken
DBusException::DBusException(const DBusException& e) throw()
{
    dbus_error_init(&_dbusError);
    dbus_set_error(&_dbusError, e._dbusError.name, "%s", e._dbusError.message);
}

//assignmen operator: special care must be taken
DBusException& DBusException::operator=(const DBusException& e) throw()
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

//Expanded description of the exception of the forme "name: description"
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
  
bool DBusException::isSet() const throw()
{
    return dbus_error_is_set(&_dbusError) == TRUE;
}

}

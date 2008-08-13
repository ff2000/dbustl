#include <dbus/dbus.h>

#include <dbustl/Connection>
#include <dbustl/Proxy>

namespace dbustl {

Proxy::Proxy(Connection* conn, const std::string& path, const std::string& destination) :
  _conn(conn), _path(path), _destination(destination), _timeout(-1)
{
}

//TODO : handle out of memory conditions
DBusMessage * Proxy::newMethodCall(const std::string& methodName)
{
    const char *dest = _destination.empty() ? NULL : _destination.c_str();
    const char *intf = _interface.empty() ? NULL : _interface.c_str();
    
    return dbus_message_new_method_call(dest, _path.c_str(), intf, methodName.c_str());
}


}

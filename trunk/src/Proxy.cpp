#include <dbus/dbus.h>

#include <dbustl/Connection>
#include <dbustl/Proxy>

namespace dbustl {

Proxy::Proxy(Connection* conn, const std::string& path, const std::string& destination) :
  _conn(conn), _path(path), _destination(destination), _timeout(-1)
{
}

DBusMessage * Proxy::newMethodCall(const std::string& methodName)
{
    const char *dest = _destination.empty() ? NULL : _destination.c_str();
    const char *intf = _interface.empty() ? NULL : _interface.c_str();
    
    return dbus_message_new_method_call(dest, _path.c_str(), intf, methodName.c_str());
}

DBusMessage* Proxy::doBlockingCall(DBusMessage *method_call)
{
    DBusException error;
    DBusMessage *reply;
          
    reply = dbus_connection_send_with_reply_and_block(_conn->dbus(), method_call, _timeout, error.dbus());
    if(!reply) {
        throw_or_set(error);
    }
    return reply;
}

bool Proxy::processCall(DBusMessage *method_call, DBusMessageIter *)
{
    if(_parsingInputArguments) {
        //All arguments are consumed but we haven't called yet, do it now
        MessageContainer reply(doBlockingCall(method_call));
        return (reply.dbus() != NULL);
    }
    return true;
}

}

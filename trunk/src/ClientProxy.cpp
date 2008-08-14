#include <dbus/dbus.h>

#include <dbustl/Connection>
#include <dbustl/ClientProxy>

namespace dbustl {

ClientProxy::ClientProxy(Connection* conn, const std::string& path, const std::string& destination) :
  _conn(conn), _path(path), _destination(destination), _timeout(-1)
{
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

bool ClientProxy::processCall(Message& method_call)
{
    if(_parsingInputArguments) {
        //All arguments are consumed but we haven't called yet, do it now
        Message reply(call(method_call));
        return !reply.isNull();
    }
    return true;
}

}

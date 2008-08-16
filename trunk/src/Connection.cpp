#include <dbus/dbus.h>

#include <dbustl/Connection>

namespace dbustl {

class ConnectionInitializer {
    ~ConnectionInitializer() {
        Connection::cleanup();  
    };
    static ConnectionInitializer _static;
};

ConnectionInitializer ConnectionInitializer::_static;

Connection* Connection::_system;
Connection* Connection::_session;

Connection* Connection::systemBus()
{
    if(!_system) {
        _system = new Connection(DBUS_BUS_SYSTEM);
    }
    return _system;
}

Connection* Connection::sessionBus()
{
    if(!_session) {
        _session = new Connection(DBUS_BUS_SESSION);
    }
    return _session;
}

DBusConnection * Connection::dbus()
{
    return _llconn;
}

Connection::Connection(DBusBusType bustype)
{
    _llconn = dbus_bus_get_private(bustype, 0);
}

Connection::~Connection()
{
    dbus_connection_close(_llconn);
    dbus_connection_unref(_llconn);
}

void Connection::cleanup() {
    delete _system;
    _system = 0;
    delete _session;
    _session = 0;
}

}

#include <dbus/dbus.h>

#include <dbustl/Connection>

namespace dbustl {

Connection* Connection::systemBus()
{
    DBusConnection *c = dbus_bus_get(DBUS_BUS_SYSTEM, 0);
    if(c) {
        return new Connection(c);
    }
    else {
        return NULL;
    }
}

Connection* Connection::sessionBus()
{
    DBusConnection *c = dbus_bus_get(DBUS_BUS_SESSION, 0);
    if(c) {
        return new Connection(c);
    }
    else {
        return NULL;
    }
}

DBusConnection * Connection::dbus()
{
    return _llconn;
}

Connection::Connection(DBusConnection* llconn)
{
    _llconn = llconn;
}

Connection::~Connection()
{
    dbus_connection_unref(_llconn);
}

}

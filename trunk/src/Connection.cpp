#include <dbus/dbus.h>

#include <dbustl/Connection>
#include <dbustl/MainLoopIntegration>
#include <cassert>

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
std::auto_ptr<MainLoopIntegration> Connection::_defaultMainLoop;

void Connection::useMainLoop(const MainLoopIntegration& mainloop)
{
    _defaultMainLoop = std::auto_ptr<MainLoopIntegration>(mainloop.construct());
}

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

DBusConnection* Connection::dbus()
{
    return _llconn;
}

Connection::Connection(DBusBusType bustype)
{
    //As per DBUS documentation, it is safe to call it more than once, as
    //calls other than the first one are ignored
    dbus_threads_init_default();
    _llconn = dbus_bus_get_private(bustype, 0);

    if(_defaultMainLoop.get()) {
        _mainLoop = std::auto_ptr<MainLoopIntegration>(_defaultMainLoop->construct());
        _mainLoop->connect(this);
    }
}

Connection::Connection(DBusBusType bustype, std::auto_ptr<MainLoopIntegration> mainLoop) : 
  _mainLoop(mainLoop)
{
    assert(_mainLoop.get());
    //As per DBUS documentation, it is safe to call it more than once, as
    //calls other than the first one are ignored
    dbus_threads_init_default();
    _llconn = dbus_bus_get_private(bustype, 0);
    
    _mainLoop->connect(this);
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
    //Release all memory allocated internally by dbus library
    dbus_shutdown();
}

}

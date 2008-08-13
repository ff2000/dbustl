#include <dbustl.h>

#include <glib.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>

#include <iostream>

int main()
{
    
    dbustl::Connection * conn = dbustl::Connection::systemBus();
    dbus_connection_setup_with_g_main(conn->dbus(), NULL);
    try {
        std::string foundService;
        dbustl::Proxy bluezManager(conn, "/org/bluez", "org.bluez");
        
        bluezManager.setInterface("org.bluez.Manager");
        bluezManager.call("FindService", "audio", &foundService);
        
        std::cout << foundService << std::endl;
    }
    catch(...) {
      delete conn;
      throw;
    }
}

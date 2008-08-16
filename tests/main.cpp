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
        dbustl::ClientProxy bluezManager(conn, "/org/bluez", "org.bluez");        

        std::string foundService;
        bluezManager.call("FindService", dbustl::ClientProxy::Interface("org.bluez.Manager"), "audio", &foundService);
        std::cout << foundService << std::endl;

        //bluezManager.call("InterfaceVersion");
    }
    catch(...) {
      delete conn;
      throw;
    }
}

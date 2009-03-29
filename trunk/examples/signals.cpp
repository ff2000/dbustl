/*
 *  DBUSTL - DBus Template Library
 *
 *  Copyright (C) 2008  Fabien Chevalier <chefabien@gmail.com>
 *  
 *
 *  This file is part of the DBus Template Library.
 *
 *  The DBus Template Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  DBus Template Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with DBus Template Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <string>

#include <dbustl-1/Glib>

class DeviceManager {
public:
    void deviceAdded(dbustl::Message m) {
        std::string devicePath;
        m >> devicePath;
        std::cout << "Device added: " << devicePath << std::endl;
    }
    void deviceRemoved(dbustl::Message m) {
        std::string devicePath;
        m >> devicePath;
        std::cout << "Deviceremoved: " << devicePath << std::endl;
    }
};

int main()
{    
    /* Setting Glib as default event loop integration method */
    dbustl::GlibEventLoopIntegration mli;
    dbustl::Connection::useEventLoop(mli);    

    dbustl::Connection *systemBus = dbustl::Connection::systemBus();

    /* Creating proxy on HAL Manager object */
    dbustl::ServerProxy halManager(systemBus, "/org/freedesktop/Hal/Manager", "org.freedesktop.Hal");

    DeviceManager deviceManager;

    halManager.setSignalHandler("DeviceAdded", &DeviceManager::deviceAdded, &deviceManager); 
    halManager.setSignalHandler("DeviceRemoved", &DeviceManager::deviceRemoved, &deviceManager); 
    
    GMainLoop *mainloop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(mainloop);
    g_main_loop_unref(mainloop);
    
    return 0;
}

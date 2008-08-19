#include <dbustl.h>
#include <dbustl/GlibMainLoopIntegration>

#include <iostream>
#include <string>
#include <cassert>

int main()
{    
    dbustl::Connection::useMainLoop(dbustl::GlibMainLoopIntegration());    
    dbustl::Connection *session = dbustl::Connection::sessionBus();
    bool exception_thrown;
    return 0;
}

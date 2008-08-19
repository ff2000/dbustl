#include <dbustl/Connection>

#include <dbustl/GlibMainLoopIntegration>
#include <iostream>

namespace dbustl {

GlibMainLoopIntegration::GlibMainLoopIntegration(GMainContext *ctxt) : _ctxt(ctxt)
{
    if(!_ctxt) {
        _ctxt = g_main_context_default();
    }
    g_main_context_ref(_ctxt);
}

GlibMainLoopIntegration::~GlibMainLoopIntegration()
{
    g_main_context_unref(_ctxt);
}

MainLoopIntegration* GlibMainLoopIntegration::construct() const
{
    return new GlibMainLoopIntegration(_ctxt);
}

void GlibMainLoopIntegration::connect(Connection*)
{
}

}

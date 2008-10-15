/*
 *  DBUSTL - DBus Template Library
 *
 *  Copyright (C) 2008  Fabien Chevalier <fabchevalier@free.fr>
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

#include <dbustl-1/Connection>

#include <dbustl-1/GlibMainLoopIntegration>
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

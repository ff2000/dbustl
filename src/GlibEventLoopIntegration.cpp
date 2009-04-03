/*
 *  DBUSTL - DBus Template Library
 *
 *  Copyright (C) 2008, 2009  Fabien Chevalier <chefabien@gmail.com>
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

#include <dbustl-1/GlibEventLoopIntegration>

#include <cassert>

namespace dbustl {

GSourceFuncs GlibEventLoopIntegration::_dispatchCallbacks = {
    GlibEventLoopIntegration::prepareDispatch,
    GlibEventLoopIntegration::checkDispatch,
    GlibEventLoopIntegration::doDispatch,
    NULL,
    NULL,
    NULL
    };

GlibEventLoopIntegration::GlibEventLoopIntegration(GMainContext *ctxt) : _ctxt(ctxt), _dispatchGSource(0)
{
    if(!_ctxt) {
        _ctxt = g_main_context_default();
    }
    g_main_context_ref(_ctxt);
}

GlibEventLoopIntegration::~GlibEventLoopIntegration()
{
    if(_dispatchGSource) {
        g_source_destroy(_dispatchGSource);
        g_source_unref(_dispatchGSource);
    }
    g_main_context_unref(_ctxt);
}

EventLoopIntegration* GlibEventLoopIntegration::clone() const
{
    return new GlibEventLoopIntegration(_ctxt);
}

bool GlibEventLoopIntegration::internalConnect(Connection* conn)
{
    _dispatchGSource = static_cast<DispatchGSource*>(
                        g_source_new(&_dispatchCallbacks, sizeof (DispatchGSource))
                        );
                        
    if(!_dispatchGSource) {
        goto error1;
    }
                                               
    _dispatchGSource->_conn = conn;
    g_source_attach(_dispatchGSource, _ctxt);

    if(!dbus_connection_set_watch_functions(conn->dbus(),
                                            addWatchCallBack,
                                            removeWatchCallBack,
                                            toggleWatchCallBack,
                                            _ctxt, NULL)) {
        goto error2;
    }

    if(!dbus_connection_set_timeout_functions(conn->dbus(),
                                              addTimeoutCallBack,
                                              removeTimeoutCallBack,
                                              toggleTimeoutCallBack,
                                              _ctxt, NULL)) {
        goto error2;
    }

    dbus_connection_set_wakeup_main_function(conn->dbus(),
					    wakeUpCallBack,
					    _ctxt, NULL);
    return true;

error2:    
    g_source_unref(_dispatchGSource);
    
error1:
    return false;
}

gboolean GlibEventLoopIntegration::prepareDispatch (GSource *source, gint *timeout)
{
    Connection *connection = static_cast<GlibEventLoopIntegration::DispatchGSource *>(source)->_conn;
  
    /* We do not force any timeout on the main loop */
    *timeout = -1;

    return (dbus_connection_get_dispatch_status (connection->dbus()) == DBUS_DISPATCH_DATA_REMAINS);
}

gboolean GlibEventLoopIntegration::checkDispatch (GSource *)
{
    return FALSE;
}

gboolean GlibEventLoopIntegration::doDispatch (GSource *source, GSourceFunc, gpointer)
{
    Connection *connection = static_cast<GlibEventLoopIntegration::DispatchGSource *>(source)->_conn;

    dbus_connection_ref (connection->dbus());

    /* Only dispatch once - we don't want to starve other GSources */
    dbus_connection_dispatch (connection->dbus());
  
    dbus_connection_unref (connection->dbus());

    return TRUE;
}

dbus_bool_t GlibEventLoopIntegration::addWatchCallBack(DBusWatch *watch, void *data)
{
    guint flags;
    int condition;
    GIOChannel *channel;
    GSource *source;
    GMainContext *ctxt = static_cast<GMainContext*>(data);
  
    if (!dbus_watch_get_enabled (watch)) {
        return TRUE;
    }
  
    assert(dbus_watch_get_data (watch) == NULL);
  
    flags = dbus_watch_get_flags (watch);

    condition = G_IO_ERR | G_IO_HUP;
    if (flags & DBUS_WATCH_READABLE)
        condition |= G_IO_IN;
    if (flags & DBUS_WATCH_WRITABLE)
        condition |= G_IO_OUT;
  
    channel = g_io_channel_unix_new (dbus_watch_get_unix_fd (watch));
    if(!channel) {
        return FALSE;
    }
  
    source = g_io_create_watch (channel, static_cast<GIOCondition>(condition));
    g_io_channel_unref (channel);
    
    if(!source) {
        return FALSE;
    }
    
    g_source_set_callback (source, reinterpret_cast<GSourceFunc>(dispatchWatch), watch,
                         NULL);
    g_source_attach (source, ctxt);
    dbus_watch_set_data (watch, source, NULL);
    
    return TRUE;
}

void GlibEventLoopIntegration::removeWatchCallBack(DBusWatch *watch, void *)
{
    GSource *source = static_cast<GSource *>(dbus_watch_get_data (watch));

    if(source) {
        g_source_destroy(source);
        g_source_unref(source);

        dbus_watch_set_data (watch, NULL, NULL);
    }
}

void GlibEventLoopIntegration::toggleWatchCallBack(DBusWatch *watch, void *data)
{
    if (dbus_watch_get_enabled (watch)) {
        addWatchCallBack(watch, data);
    }
    else {
        removeWatchCallBack(watch, data);
    }
}

gboolean GlibEventLoopIntegration::dispatchWatch(GIOChannel *, GIOCondition condition, gpointer data)
{
    DBusWatch *watch = static_cast<DBusWatch *>(data);
    unsigned int dbus_condition = 0;

    if (condition & G_IO_IN)
        dbus_condition |= DBUS_WATCH_READABLE;
    if (condition & G_IO_OUT)
        dbus_condition |= DBUS_WATCH_WRITABLE;
    if (condition & G_IO_ERR)
        dbus_condition |= DBUS_WATCH_ERROR;
    if (condition & G_IO_HUP)
        dbus_condition |= DBUS_WATCH_HANGUP;

    dbus_watch_handle(watch, dbus_condition);
  
    return TRUE;
}

dbus_bool_t GlibEventLoopIntegration::addTimeoutCallBack(DBusTimeout *timeout, void *data)
{
    GSource *source;
    GMainContext *ctxt = static_cast<GMainContext*>(data);
  
    if (!dbus_timeout_get_enabled (timeout)) {
      return TRUE;
    }
  
    assert(dbus_timeout_get_data (timeout) == NULL);

    source = g_timeout_source_new (dbus_timeout_get_interval (timeout));
    if(!source) {
      return FALSE;
    }
  
    g_source_set_callback (source, dispatchTimeout, timeout,
                         NULL);
    g_source_attach (source, ctxt);

    dbus_timeout_set_data (timeout, source, NULL);
    
    return TRUE;
}

void GlibEventLoopIntegration::removeTimeoutCallBack(DBusTimeout *timeout, void *)
{
    GSource *source = static_cast<GSource *>(dbus_timeout_get_data(timeout));

    if(source) {
        g_source_destroy(source);
        g_source_unref(source);

        dbus_timeout_set_data (timeout, NULL, NULL);
    }
}

void GlibEventLoopIntegration::toggleTimeoutCallBack(DBusTimeout *timeout, void *data)
{
    if (dbus_timeout_get_enabled (timeout)) {
        addTimeoutCallBack(timeout, data);
    }
    else {
        removeTimeoutCallBack(timeout, data);
    }
}

gboolean GlibEventLoopIntegration::dispatchTimeout(gpointer data)
{
    DBusTimeout *timeout = static_cast<DBusTimeout *>(data);

    dbus_timeout_handle(timeout);
  
    return TRUE;
}

void GlibEventLoopIntegration::wakeUpCallBack(void *data)
{
    GMainContext *ctxt = static_cast<GMainContext*>(data);

    g_main_context_wakeup(ctxt);
}

}

#We have separate directory for headers
AM_CXXFLAGS = -Iinclude @DBUS_CFLAGS@
AM_LDFLAGS = @DBUS_LIBS@

lib_LTLIBRARIES = libdbustl.la
libdbustl_la_SOURCES = src/ClientProxy.cpp \
				   src/Connection.cpp \
				   src/DBusException.cpp \
				   src/Message.cpp

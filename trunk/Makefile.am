SUBDIRS = . tests

#Common flags definitions
AM_CXXFLAGS = @CXX0X_CFLAGS@ -W -Wall -Iinclude @DBUS_CFLAGS@
#Common libraries
LDADD = @DBUS_LIBS@

lib_LTLIBRARIES = libdbustl-0.la
libdbustl_0_ladir = $(includedir)/dbustl
libdbustl_0_la_HEADERS = include/dbustl/dbustl \
                   include/dbustl/ClientProxy \
                   include/dbustl/Connection \
                   include/dbustl/DBusException \
                   include/dbustl/MainLoopIntegration \
                   include/dbustl/Message \
                   include/dbustl/StdTypesConvertion \
                   include/dbustl/TypesConvertion
libdbustl_0_la_SOURCES = src/ClientProxy.cpp \
                   src/Connection.cpp \
                   src/DBusException.cpp \
                   src/Message.cpp

if HAVE_GLIB
lib_LTLIBRARIES += libdbustl-glib-0.la
endif

libdbustl_glib_0_la_CPPFLAGS = @GLIB_CFLAGS@
libdbustl_glib_0_la_LIBADD = @GLIB_LIBS@
libdbustl_glib_0_ladir = $(includedir)/dbustl
libdbustl_glib_0_la_HEADERS = include/dbustl/Glib \
   	               			include/dbustl/GlibMainLoopIntegration
libdbustl_glib_0_la_SOURCES = src/GlibMainLoopIntegration.cpp

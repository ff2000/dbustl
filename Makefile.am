SUBDIRS = . tests

#License files
EXTRA_DIST = COPYING COPYING.LESSER

#Common flags definitions
AM_CXXFLAGS = @CXX0X_CFLAGS@ -W -Wall -Iinclude @DBUS_CFLAGS@
#Common libraries
LDADD = @DBUS_LIBS@

lib_LTLIBRARIES = libdbustl-1.la
libdbustl_1_ladir = $(includedir)/dbustl-1
libdbustl_1_la_HEADERS = include/dbustl-1/Config \
				   include/dbustl-1/dbustl \
                   include/dbustl-1/ClientProxy \
                   include/dbustl-1/Connection \
                   include/dbustl-1/DBusException \
                   include/dbustl-1/MainLoopIntegration \
                   include/dbustl-1/Message \
                   include/dbustl-1/StdTypesConvertion \
                   include/dbustl-1/TypesConvertion
libdbustl_1_la_SOURCES = src/ClientProxy.cpp \
                   src/Connection.cpp \
                   src/DBusException.cpp \
                   src/Message.cpp

#dbustl pkg-config support
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = dbustl-1.pc
DISTCLEANFILES = dbustl-1.pc
EXTRA_DIST     = dbustl-1.pc.in           

if HAVE_GLIB
lib_LTLIBRARIES += libdbustl-glib-1.la
endif

libdbustl_glib_1_la_CPPFLAGS = @GLIB_CFLAGS@
libdbustl_glib_1_la_LIBADD = @GLIB_LIBS@
libdbustl_glib_1_ladir = $(includedir)/dbustl-1
libdbustl_glib_1_la_HEADERS = include/dbustl-1/Glib \
   	               			include/dbustl-1/GlibMainLoopIntegration
libdbustl_glib_1_la_SOURCES = src/GlibMainLoopIntegration.cpp

#dbustl-glib pkg-config support
if HAVE_GLIB
pkgconfig_DATA += dbustl-glib-1.pc
endif
DISTCLEANFILES += dbustl-glib-1.pc
EXTRA_DIST     += dbustl-glib-1.pc.in           


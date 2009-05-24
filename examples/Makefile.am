#Common flags definitions
AM_CXXFLAGS = @CXX0X_CFLAGS@  -I../include -W -Wall @DBUS_CFLAGS@
#Common libraries
LDADD = @DBUS_LIBS@ ../libdbustl-1.la

noinst_PROGRAMS = compat
compat_SOURCES = compat.cpp

if HAVE_STDCXX_0X
noinst_PROGRAMS += simplest containers struct
endif
simplest_SOURCES = simplest.cpp
containers_SOURCES = containers.cpp
struct_SOURCES = struct.cpp struct.h

if HAVE_GLIB
noinst_PROGRAMS += signals
endif
signals_SOURCES = signals.cpp
signals_CPPFLAGS = @GLIB_CFLAGS@
signals_LDADD = $(LDADD) ../libdbustl-glib-1.la @GLIB_LIBS@

if HAVE_STDCXX_0X
if HAVE_GLIB
noinst_PROGRAMS += service
endif
endif
service_SOURCES = service.cpp
service_CPPFLAGS = @GLIB_CFLAGS@
service_LDADD = $(LDADD) ../libdbustl-glib-1.la @GLIB_LIBS@

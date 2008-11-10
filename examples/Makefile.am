#Common flags definitions
AM_CXXFLAGS = @CXX0X_CFLAGS@  -I../include -W -Wall @DBUS_CFLAGS@
#Common libraries
LDADD = @DBUS_LIBS@ ../libdbustl-1.la

noinst_PROGRAMS = compat containers
compat_SOURCES = compat.cpp
containers_SOURCES = containers.cpp

if HAVE_STDCXX_0X
noinst_PROGRAMS += simplest
endif
simplest_SOURCES = simplest.cpp

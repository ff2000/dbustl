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
struct_SOURCES = struct1.cpp struct2.cpp struct.h

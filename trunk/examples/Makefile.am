#Common flags definitions
AM_CXXFLAGS = @CXX0X_CFLAGS@  -I../include -W -Wall @DBUS_CFLAGS@
#Common libraries
LDADD = @DBUS_LIBS@ ../libdbustl-1.la

if HAVE_STDCXX_0X
noinst_PROGRAMS = simplest
endif

simplest_SOURCES = simplest.cpp

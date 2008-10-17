#Common flags definitions
AM_CXXFLAGS = @CXX0X_CFLAGS@  -I../include -W -Wall @DBUS_CFLAGS@
#Common libraries
LDADD = @DBUS_LIBS@ ../libdbustl-1.la

noinst_PROGRAMS = standard-tests
standard_tests_SOURCES = standard-tests.cpp

if HAVE_GLIB
noinst_PROGRAMS += async-tests
endif
async_tests_SOURCES = async-tests.cpp
async_tests_CPPFLAGS = @GLIB_CFLAGS@
async_tests_LDADD = $(LDADD) ../libdbustl-glib-1.la @GLIB_LIBS@
#FIXME: remove once we do not need dbus-glib anymore
async_tests_LDFLAGS = `pkg-config --libs dbus-glib-1`

EXTRA_DIST = test-service.py

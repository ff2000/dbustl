#Common flags definitions
AM_CXXFLAGS = @CXX0X_CFLAGS@  -I../include -W -Wall @DBUS_CFLAGS@

noinst_PROGRAMS = link-test
link_test_SOURCES = link-test-1.cpp link-test-2.cpp
link_test_LDADD = @DBUS_LIBS@ ../libdbustl-1.la

noinst_PROGRAMS += standard-tests
standard_tests_SOURCES = standard-tests.cpp
standard_tests_LDADD = @DBUS_LIBS@ ../libdbustl-1.la

noinst_PROGRAMS += standard-tests-noex
standard_tests_noex_CPPFLAGS = -DDBUSTL_NO_EXCEPTIONS -fno-exceptions
standard_tests_noex_SOURCES = standard-tests.cpp
standard_tests_noex_LDADD = @DBUS_LIBS@ ../libdbustl-noex-1.la

if HAVE_GLIB
noinst_PROGRAMS += async-tests async-tests-noex server
endif

async_tests_SOURCES = async-tests.cpp
async_tests_CPPFLAGS = @GLIB_CFLAGS@
async_tests_LDADD = @DBUS_LIBS@ ../libdbustl-1.la ../libdbustl-glib-1.la @GLIB_LIBS@

async_tests_noex_SOURCES = async-tests.cpp
async_tests_noex_CPPFLAGS = @GLIB_CFLAGS@ -DDBUSTL_NO_EXCEPTIONS -fno-exceptions
async_tests_noex_LDADD = @DBUS_LIBS@ ../libdbustl-noex-1.la ../libdbustl-noex-glib-1.la @GLIB_LIBS@

server_SOURCES = server.cpp
server_CPPFLAGS = @GLIB_CFLAGS@
server_LDADD = @DBUS_LIBS@ ../libdbustl-1.la ../libdbustl-glib-1.la @GLIB_LIBS@

EXTRA_DIST = test-service.py server-tests.py introspection.txt

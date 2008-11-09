AC_PREREQ(2.59)

AC_INIT([dbustl], [0.2], [fabchevalier@free.fr])
AC_CONFIG_AUX_DIR([config])

AM_INIT_AUTOMAKE([1.9 foreign])
AM_MAINTAINER_MODE

AC_PROG_CXX
#Disable libtool shared libraries support, we are a template library afterall !!
AC_DISABLE_SHARED
AC_PROG_LIBTOOL
#Check for optionnal C++0x support
AC_CXX_COMPILE_STDCXX_0X
AC_SUBST(CXX0X_CFLAGS)

#BEGIN check for DBUS
PKG_CHECK_MODULES(DBUS, dbus-1 >= 1.2, have_dbus=yes, have_dbus=no)

if test x$have_dbus = xno ; then
    AC_MSG_ERROR([DBus development libraries >=1.2 not found])
fi

AC_SUBST(DBUS_CFLAGS)
AC_SUBST(DBUS_LIBS)
#END check for DBUS

#BEGIN check for GLIB
PKG_CHECK_MODULES(GLIB, glib-2.0 >= 2.0, have_glib=yes, have_glib=no)
AM_CONDITIONAL(HAVE_GLIB, test x$have_glib = xyes)
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)
#END check for GLIB

AC_CONFIG_FILES([Makefile
                 include/Makefile 
                 tests/Makefile
                 examples/Makefile
                 dbustl-1.pc
                 dbustl-glib-1.pc
                 Doxyfile])
AC_OUTPUT

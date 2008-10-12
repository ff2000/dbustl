AC_PREREQ(2.59)

AC_INIT([dbustl], [0.1], [fabchevalier@free.fr])
AC_CONFIG_AUX_DIR([config])

AM_INIT_AUTOMAKE([1.9 foreign])
AM_MAINTAINER_MODE

AC_PROG_CXX
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(DBUS, dbus-1 >= 1.2, have_dbus=yes, have_dbus=no)

if test x$have_dbus = xno ; then
    AC_MSG_ERROR([DBus development libraries >=1.2 not found])
fi
AM_CONDITIONAL(HAVE_DBUS, test x$have_dbus = xyes)

AC_SUBST(DBUS_CFLAGS)
AC_SUBST(DBUS_LIBS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

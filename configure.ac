#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])

m4_include([m4/version.m4])

AC_INIT([nomnom], [_NOMNOM_VERSION],
  [http://sourceforge.net/apps/trac/nomnom], [],
  [http://nomnom.sourceforge.net/])

AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([config.aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_SYSTEM
AC_GNU_SOURCE

AC_DEFINE_UNQUOTED([CANONICAL_TARGET], "$target", [...])
AC_DEFINE_UNQUOTED([CANONICAL_HOST], "$host", [...])

AM_INIT_AUTOMAKE([-Wall -Werror foreign dist-bzip2 dist-xz])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([QtCore],   [QtCore >= 4.6.0])
PKG_CHECK_MODULES([QtGui],    [QtGui >= 4.6.0])
PKG_CHECK_MODULES([QJson],    [QJson >= 0.7.1])

# --enable-libproxy
#AC_ARG_ENABLE([libproxy],
#  [AS_HELP_STRING([--enable-libproxy], [Build with libproxy])])
#
#AM_CONDITIONAL([ENABLE_LIBPROXY], [test -n "$enable_libproxy"])
#
#AS_IF([test -n "$enable_libproxy"], [
#  PKG_CHECK_MODULES([libproxy], [libproxy-1.0 >= 0.4.7], [], [
#    PKG_CHECK_MODULES([libproxy], [libproxy >= 0.4.7])])
#  AC_DEFINE([HAVE_LIBPROXY], [1], [We have libproxy])
#])

# Checks for Qt programs.

LRELEASE=`$PKG_CONFIG --variable=lrelease_location QtCore 2>/dev/null`
MOC=`$PKG_CONFIG --variable=moc_location QtCore 2>/dev/null`
RCC=`$PKG_CONFIG --variable=rcc_location QtCore 2>/dev/null`
UIC=`$PKG_CONFIG --variable=uic_location QtCore 2>/dev/null`

AS_IF([test -z "$MOC"], [
  AC_MSG_ERROR([QtCore.pc: moc_location not found, is qt installed?])])

AS_IF([test -z "$UIC"], [
  AC_MSG_ERROR([QtCore.pc: uic_location not found, is qt installed?])])

# debian: QtCore.pc (4.6.3): rcc_location undefined in QtCore.pc
# rcc: required. You can set RCC_PATH to point to a directory with lrelease.
AS_IF([test -z "$RCC"], [
  AC_PATH_PROGS([RCC], [rcc rcc-qt4], [], [$RCC_PATH$PATH_SEPARATOR$PATH])
  AS_IF([test -z "$RCC"], [AC_MSG_ERROR([rcc not found, is qt installed?])])
])

# debian: QtCore.pc (4.6.3): lrelease_location undefined in QtCore.pc
# lrelease: optional: translations will not be installed.
# You can set LRELEASE_PATH to point to a directory with lrelease.
AS_IF([test -z "$LRELEASE"], [
  AC_PATH_PROGS([LRELEASE], [lrelease lrelease-qt4], [],
    [$LRELEASE_PATH$PATH_SEPARATOR$PATH])
  AS_IF([test -z "$LRELEASE"], [
    AC_MSG_NOTICE([lrelease not found, translations will not be built])])
])
AM_CONDITIONAL([HAVE_LRELEASE], [test -n "$LRELEASE"])

AC_SUBST([LRELEASE])
AC_SUBST([MOC])
AC_SUBST([RCC])
AC_SUBST([UIC])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

# Version.
VN=`$srcdir/gen-ver.sh $srcdir`
AC_DEFINE_UNQUOTED([VN],["$VN"], [We have version number from gen-ver.sh])

# --enable-verbose
AC_ARG_ENABLE([verbose],
  AS_HELP_STRING([--enable-verbose], [Enable --verbose program option]))

AS_IF([test "x$enable_verbose" = "xyes"], [
  AC_DEFINE([ENABLE_VERBOSE], [1], [Enable --verbose option])])

# --with-manual
AC_ARG_WITH([manual],
  [AS_HELP_STRING([--with-manual],
    [Install manual page(s) @<:@default=yes@:>@])],
  [],
  [with_manual=yes])
AM_CONDITIONAL([WITH_MANUAL], [test x"$with_manual" != "xno"])

AC_CONFIG_FILES([
  Makefile
  doc/Makefile
  doc/man1/Makefile
  tr/Makefile
  src/Makefile
  nomnom.desktop
])

AC_OUTPUT

AC_MSG_NOTICE([
 nomnom version:  ${VERSION} (${VN})
 host setup:      ${host}
options:
 install prefix:  ${prefix}
 with manual:     ${with_manual}
 --verbose:       ${enable_verbose}
qt:
 lrelease:        ${LRELEASE}
 moc:             ${MOC}
 rcc:             ${RCC}
 uic:             ${UIC}])

# vim: set ts=2 sw=2 tw=72 expandtab:

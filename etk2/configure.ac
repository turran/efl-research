# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT(etk2, 0.1.0.0, hisham.mardambey@gmail.com)
AC_PREREQ(2.52)
AC_CONFIG_SRCDIR(configure.ac)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ISC_POSIX

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(config.h)

AC_C_BIGENDIAN
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST
AM_ENABLE_SHARED
AM_PROG_LIBTOOL

dnl Add the languages which your application supports here.

if test "x${bindir}" = 'x${exec_prefix}/bin'; then
  if test "x${exec_prefix}" = "xNONE"; then
    if test "x${prefix}" = "xNONE"; then
      bindir="${ac_default_prefix}/bin";
    else
      bindir="${prefix}/bin";
    fi
  else
    if test "x${prefix}" = "xNONE"; then
      bindir="${ac_default_prefix}/bin";
    else
      bindir="${prefix}/bin";
    fi
  fi
fi

if test "x${libdir}" = 'x${exec_prefix}/lib'; then
  if test "x${exec_prefix}" = "xNONE"; then
    if test "x${prefix}" = "xNONE"; then
      libdir="${ac_default_prefix}/lib";
    else
      libdir="${prefix}/lib";
    fi
  else
    if test "x${prefix}" = "xNONE"; then
      libdir="${ac_default_prefix}/lib";
    else
      libdir="${prefix}/lib";
    fi
  fi
fi

dnl Set PACKAGE_DATA_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${ac_default_prefix}/share/${PACKAGE}", [Shared Data Directory])
else
  AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${prefix}/share/${PACKAGE}", [Shared Data Directory])
fi

dnl Set PACKAGE_BIN_DIR in config.h.
if test "x${bindir}" = 'xNONE'; then
  if test "x${prefix}" = "xNONE"; then
    AC_DEFINE_UNQUOTED(PACKAGE_BIN_DIR, "${ac_default_prefix}/bin", [Installation Directory for User Executables])
  else
    AC_DEFINE_UNQUOTED(PACKAGE_BIN_DIR, "${prefix}/bin", [Installation Directory for User Executables])
  fi
else
  AC_DEFINE_UNQUOTED(PACKAGE_BIN_DIR, "${bindir}", [Installation Directory for User Executables])
fi

dnl Set PACKAGE_LIB_DIR in config.h.
if test "x${libdir}" = 'xNONE'; then
  if test "x${prefix}" = "xNONE"; then
    AC_DEFINE_UNQUOTED(PACKAGE_LIB_DIR, "${ac_default_prefix}/lib", [Installation Directory for Libraries])
  else
    AC_DEFINE_UNQUOTED(PACKAGE_LIB_DIR, "${prefix}/lib", [Installation Directory for Libraries])
  fi
else
  AC_DEFINE_UNQUOTED(PACKAGE_LIB_DIR, "${libdir}", [Installation Directory for Libraries])
fi

dnl Set PACKAGE_SOURCE_DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`
AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}", [Source Code Directory])

case "$host_os" in
    *bsd*)
       AC_DEFINE(HAVE_BSD, [], "Have a BSD distribution")
    ;;
    linux*)
       AC_DEFINE(HAVE_LINUX, [], "Have a Linux distribution")
    ;;
esac

requirements="eina evas ecore ecore-file ecore-evas ecore-imf ecore-imf-evas edje"

PKG_CHECK_MODULES(EINA, [eina-0 >= 0.0.1])
PKG_CHECK_MODULES(EVAS, [evas >= 0.9.9])
PKG_CHECK_MODULES(ECORE, [ecore >= 0.9.9 ecore-file >= 0.9.9 ecore-evas >= 0.9.9 ecore-imf >= 0.9.9 ecore-imf-evas >= 0.9.9])
PKG_CHECK_MODULES(EDJE, [edje >= 0.5.0])

#####################################################################
## Ecore_X
PKG_CHECK_MODULES(ECORE_X, [ecore-x >= 0.9.9], [
    have_ecore_x=yes
    AC_DEFINE(HAVE_ECORE_X, 1, [Define to 1 if you have the Ecore_X headers and libraries])
    requirements="$requirements ecore-x"
  ], [
    have_ecore_x=no
    AC_DEFINE(HAVE_ECORE_X, 0, [Define to 1 if you have the Ecore_X headers and libraries])
])

AM_CONDITIONAL(BUILD_ENGINE_EE_S_X11, test "x$have_ecore_x" = "xyes")

#####################################################################
## Ecore_FB
PKG_CHECK_MODULES(ECORE_FB, [ecore-fb >= 0.9.9], [
    have_ecore_fb=yes
    AC_DEFINE(HAVE_ECORE_FB, 1, [Define to 1 if you have the Ecore_FB headers and libraries])
    requirements="$requirements ecore-fb"
  ], [
    have_ecore_fb=no
    AC_DEFINE(HAVE_ECORE_FB, 0, [Define to 1 if you have the Ecore_FB headers and libraries])
])

AC_ARG_ENABLE(ecore-fb-x11-support,
  AC_HELP_STRING(
    [--enable-ecore-fb-x11-support],
    [enable the ecore_fb engine X11 support. [[default=enabled]]]),
  [ want_ecore_fb_x11_support=$enableval ],
  [ want_ecore_fb_x11_support="yes" ])

AC_MSG_CHECKING(whether engine ecore_fb with X11 support is to be built)
AC_MSG_RESULT($want_ecore_fb_x11_support)

if test "x$want_ecore_fb_x11_support" = "xyes"; then
  if test "x$have_ecore_fb" = "xyes" -a "x$have_ecore_x" = "xyes"; then
    have_ecore_fb_x11_support=yes
    AC_DEFINE(ENGINE_E_FB_X11_SUPPORT, [1], [Build support for X11 on ecore_fb engine])
  else
    have_ecore_fb_x11_support=no
    AC_MSG_WARN(Silly monkey: engine ecore_fb X11 support requires ecore_fb and ecore_x ... disabling ecore_fb X11 support)
  fi
fi

AM_CONDITIONAL(BUILD_ENGINE_E_FB, test "x$have_ecore_fb" = "xyes")

#####################################################################
## Fill in flags
AC_SUBST(requirements)

AC_SUBST(packagesrcdir)

#####################################################################
## Output

AC_OUTPUT([
Makefile
etk2.pc
src/Makefile
src/lib/Makefile
src/engines/Makefile
src/engines/ecore_evas/Makefile
src/bin/Makefile
])

#####################################################################
## Info

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo
echo
echo "Configuration Options Summary:"
echo
echo "  Ecore Evas Software X11 Engine..............: $have_ecore_x"
echo "  Ecore FB Engine.............................: $have_ecore_fb"
echo "  Ecore FB Engine X11 Support.................: $have_ecore_fb_x11_support"
echo
echo "Installation Path.........: $prefix"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE,"
echo "and then afterwards as root (or the user who will install this), type"
echo "'make install'. Change users with 'su' or 'sudo' appropriately."
echo

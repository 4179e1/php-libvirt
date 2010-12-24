PHP_ARG_ENABLE(libvirt, whether to enable Libvirt support,
[ --enable-libvirt   Enable Libvirt support])

if test "$PHP_LIBVIRT" = "yes"; then
    for i in /usr /usr/local /opt; do
    if test -f $i/include/libvirt/libvirt.h; then
      LIBVIRT_DIR=$i
    fi
  done

  if test -z "$LIBVIRT_DIR"; then
    AC_MSG_ERROR([libvirt not installed (http://www.libvirt.org/)])
  fi

  PHP_ADD_LIBRARY_WITH_PATH(virt,$LIBVIRT_DIR/lib, LIBVIRT_SHARED_LIBADD)
  PHP_ADD_INCLUDE($LIBVIRT_DIR/include)

  AC_MSG_RESULT([found in $LIBVIRT_DIR])
  AC_DEFINE(HAVE_LIBVIRT, 1, [Whether you have Libvirt])

  PHP_NEW_EXTENSION(libvirt, libvirt.c, $ext_shared)
  PHP_SUBST(LIBVIRT_SHARED_LIBADD)
fi

ac_cv_enable_getconnect="no"
AC_ARG_ENABLE([getconnect],
		[  --enable-getconnect [default=disabled]],
		[ac_cv_enable_getconnect=$enableval], [ac_cv_enable_getconnect=no])
AC_CACHE_CHECK([whetehr to enable *get_connect functions], [ac_cv_enable_getconnect], [ac_cv_enable_getconnect=yes])
if test "$ac_cv_enable_getconnect" = yes; then
	AC_DEFINE([HAVE_GETCONNECT], [1], [Define to enable *get_connect functions])
fi

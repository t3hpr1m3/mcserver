AC_PREREQ([2.69])
AC_INIT([mcserver], [0.1.0], [vmizzle@gmail.com])
m4_include([m4/ax_jni_include_dir.m4])
AC_CONFIG_SRCDIR([src/globals.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_CC

AC_CHECK_HEADER_STDBOOL

dnl Check for Java
AX_JNI_INCLUDE_DIR
for JNI_INCLUDE_DIR in $JNI_INCLUDE_DIRS
do
	CPPFLAGS="$CPPFLAGS -I$JNI_INCLUDE_DIR -I$JNI_INCLUDE_DIR/linux"
done

AC_CHECK_LIB([dl], [dlopen])

AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT

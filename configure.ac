AC_PREREQ([2.60])
AC_INIT([my_ld.so],
        [1.0],
        [nathan.gauer@epita.fr],
        [my_ld.so])

AC_USE_SYSTEM_EXTENSIONS

AC_CONFIG_SRCDIR([src])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([
  foreign
  -Wall
  silent-rules
  subdir-objects
])

AC_PROG_CXX # ([clang++])
AM_PROG_AS

AM_SILENT_RULES([yes])

my_flags="                            \
          -Wall                       \
          -Werror                     \
          -Wextra                     \
          -O0                         \
          -g                          \
          -std=c++14                  \
          -fPIC                       \
          -nostdinc                   \
          -ffreestanding              \
          -fno-exceptions             \
          -fvisibility=hidden         \
          -iquote ./                  \
          -fno-builtin                \
          -nostdlib                   \
          -nodefaultlibs              \
          -fno-rtti                   \
          -fno-builtin-function       \
          "
AC_SUBST([my_flags])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
AC_MSG_RESULT([
  $PACKAGE $VERSION
  ====================
  prefix:    ${prefix}
  compiler:  ${CXX}
  cxxflags:  ${CXXFLAGS} ${flags}
  ldflags:   ${LDFLAGS}
])

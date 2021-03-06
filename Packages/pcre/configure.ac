# PCRE configure.ac: This version has been optimised for Plato.

m4_define(pcre_major, [7])
m4_define(pcre_minor, [6])
m4_define(pcre_prerelease, [])
m4_define(pcre_date, [2008-01-28])

AC_PREREQ(2.57)
AC_INIT(PCRE, pcre_major.pcre_minor[]pcre_prerelease, , pcre)
AC_CONFIG_SRCDIR([pcre_compile.c])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS(config.h)

# The default CFLAGS and CXXFLAGS in Autoconf are "-g -O2" for gcc and just
# "-g" for any other compiler. There doesn't seem to be a standard way of
# getting rid of the -g (which I don't think is needed for a production
# library). This fudge seems to achieve the necessary. First, we remember the
# externally set values of CFLAGS and CXXFLAGS. Then call the AC_PROG_CC and
# AC_PROG_CXX macros to find the compilers - if CFLAGS and CXXFLAGS are not
# set, they will be set to Autoconf's defaults. Afterwards, if the original
# values were not set, remove the -g from the Autoconf defaults.
# (PH 02-May-07)

remember_set_CFLAGS="$CFLAGS"

AC_PROG_CC

if test "x$remember_set_CFLAGS" = "x"
then
  if test "$CFLAGS" = "-g -O2"
  then
    CFLAGS="-O2"
  elif test "$CFLAGS" = "-g"
  then
    CFLAGS=""
  fi
fi

AC_PROG_RANLIB

PCRE_MAJOR="pcre_major"
PCRE_MINOR="pcre_minor"
PCRE_PRERELEASE="pcre_prerelease"
PCRE_DATE="pcre_date"

AC_SUBST(PCRE_MAJOR)
AC_SUBST(PCRE_MINOR)
AC_SUBST(PCRE_PRERELEASE)
AC_SUBST(PCRE_DATE)

# Handle --disable-cpp
enable_cpp=no

# Handle --enable-rebuild-chartables
enable_rebuild_chartables=no

# Handle --enable-utf8 (disabled by default)
enable_utf8=no

# Handle --enable-unicode-properties
enable_unicode_properties=no

# Handle --enable-newline=NL
dnl AC_ARG_ENABLE(newline,
dnl               AS_HELP_STRING([--enable-newline=NL],
dnl                              [use NL as newline (lf, cr, crlf, anycrlf, any; default=lf)]),
dnl               , enable_newline=lf)

# Separate newline options
ac_pcre_newline=lf
AC_ARG_ENABLE(newline-is-cr,
              AS_HELP_STRING([--enable-newline-is-cr],
                             [use CR as newline character]),
              ac_pcre_newline=cr)
AC_ARG_ENABLE(newline-is-lf,
              AS_HELP_STRING([--enable-newline-is-lf],
                             [use LF as newline character (default)]),
              ac_pcre_newline=lf)
AC_ARG_ENABLE(newline-is-crlf,
              AS_HELP_STRING([--enable-newline-is-crlf],
                             [use CRLF as newline sequence]),
              ac_pcre_newline=crlf)
AC_ARG_ENABLE(newline-is-anycrlf,
              AS_HELP_STRING([--enable-newline-is-anycrlf],
                             [use CR, LF, or CRLF as newline sequence]),
              ac_pcre_newline=anycrlf)
AC_ARG_ENABLE(newline-is-any,
              AS_HELP_STRING([--enable-newline-is-any],
                             [use any valid Unicode newline sequence]),
              ac_pcre_newline=any)
enable_newline="$ac_pcre_newline"

# Handle --enable-bsr-anycrlf
AC_ARG_ENABLE(bsr-anycrlf,
              AS_HELP_STRING([--enable-bsr-anycrlf],
                             [\R matches only CR, LF, CRLF by default]),
              , enable_bsr_anycrlf=no)

# Handle --enable-ebcdic
enable_ebcdic=no

# Handle --disable-stack-for-recursion
AC_ARG_ENABLE(stack-for-recursion,
              AS_HELP_STRING([--disable-stack-for-recursion],
                             [don't use stack recursion when matching]),
              , enable_stack_for_recursion=yes)

# Handle --enable-pcregrep-libz
enable_pcregrep_libz=no

# Handle --enable-pcregrep-libbz2
enable_pcregrep_libbz2=no

# Handle --enable-pcretest-libreadline
enable_pcretest_libreadline=no

# Handle --with-posix-malloc-threshold=NBYTES
AC_ARG_WITH(posix-malloc-threshold,
            AS_HELP_STRING([--with-posix-malloc-threshold=NBYTES],
                           [threshold for POSIX malloc usage (default=10)]),
            , with_posix_malloc_threshold=10)

# Handle --with-link-size=N
AC_ARG_WITH(link-size,
            AS_HELP_STRING([--with-link-size=N],
                           [internal link size (2, 3, or 4 allowed; default=2)]),
            , with_link_size=2)

# Handle --with-match-limit=N
AC_ARG_WITH(match-limit,
            AS_HELP_STRING([--with-match-limit=N],
                           [default limit on internal looping (default=10000000)]),
            , with_match_limit=10000000)

# Handle --with-match-limit_recursion=N
#
# Note: In config.h, the default is to define MATCH_LIMIT_RECURSION
# symbolically as MATCH_LIMIT, which in turn is defined to be some numeric
# value (e.g. 10000000). MATCH_LIMIT_RECURSION can otherwise be set to some
# different numeric value (or even the same numeric value as MATCH_LIMIT,
# though no longer defined in terms of the latter).
#
AC_ARG_WITH(match-limit-recursion,
            AS_HELP_STRING([--with-match-limit-recursion=N],
                           [default limit on internal recursion (default=MATCH_LIMIT)]),
            , with_match_limit_recursion=MATCH_LIMIT)

# Convert the newline identifier into the appropriate integer value.
case "$enable_newline" in
  lf)      ac_pcre_newline_value=10   ;;
  cr)      ac_pcre_newline_value=13   ;;
  crlf)    ac_pcre_newline_value=3338 ;;
  anycrlf) ac_pcre_newline_value=-2   ;;
  any)     ac_pcre_newline_value=-1   ;;
  *)
  AC_MSG_ERROR([invalid argument \"$enable_newline\" to --enable-newline option])
  ;;
esac

# Check argument to --with-link-size
case "$with_link_size" in
  2|3|4) ;;
  *)
  AC_MSG_ERROR([invalid argument \"$with_link_size\" to --with-link-size option])
  ;;
esac

AH_TOP([
/* On Unix-like systems config.h.in is converted by "configure" into config.h.
Some other environments also support the use of "configure". PCRE is written in
Standard C, but there are a few non-standard things it can cope with, allowing
it to run on SunOS4 and other "close to standard" systems.

If you are going to build PCRE "by hand" on a system without "configure" you
should copy the distributed config.h.generic to config.h, and then set up the
macro definitions the way you need them. You must then add -DHAVE_CONFIG_H to
all of your compile commands, so that config.h is included at the start of
every source.

Alternatively, you can avoid editing by using -D on the compiler command line
to set the macro values. In this case, you do not have to set -DHAVE_CONFIG_H.

PCRE uses memmove() if HAVE_MEMMOVE is set to 1; otherwise it uses bcopy() if
HAVE_BCOPY is set to 1. If your system has neither bcopy() nor memmove(), set
them both to 0; an emulation function will be used. */])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(limits.h sys/types.h sys/stat.h dirent.h windows.h)

# The files below are C++ header files.
pcre_have_type_traits="0"
pcre_have_bits_type_traits="0"

# Using AC_SUBST eliminates the need to include config.h in a public .h file
AC_SUBST(pcre_have_type_traits)
AC_SUBST(pcre_have_bits_type_traits)

# Checks for typedefs, structures, and compiler characteristics.

AC_C_CONST
AC_TYPE_SIZE_T

AC_CHECK_TYPES([long long],
               [pcre_have_long_long="1"],
               [pcre_have_long_long="0"])
AC_CHECK_TYPES([unsigned long long],
               [pcre_have_ulong_long="1"],
               [pcre_have_ulong_long="0"])
AC_SUBST(pcre_have_long_long)
AC_SUBST(pcre_have_ulong_long)

# Checks for library functions.

AC_CHECK_FUNCS(bcopy memmove strerror strtoq strtoll _strtoi64)

# This facilitates -ansi builds under Linux
dnl AC_DEFINE([_GNU_SOURCE], [], [Enable GNU extensions in glibc])

AC_DEFINE([PCRE_STATIC], [1], [
    Define if linking statically (TODO: make nice with Libtool)])

# Here is where pcre specific defines are handled

if test "$enable_stack_for_recursion" = "no"; then
  AC_DEFINE([NO_RECURSE], [], [
    PCRE uses recursive function calls to handle backtracking while
    matching. This can sometimes be a problem on systems that have
    stacks of limited size. Define NO_RECURSE to get a version that
    doesn't use recursion in the match() function; instead it creates
    its own stack by steam using pcre_recurse_malloc() to obtain memory
    from the heap. For more detail, see the comments and other stuff
    just above the match() function. On systems that support it,
    "configure" can be used to set this in the Makefile
    (use --disable-stack-for-recursion).])
fi

AC_DEFINE_UNQUOTED([NEWLINE], [$ac_pcre_newline_value], [
  The value of NEWLINE determines the newline character sequence. On
  systems that support it, "configure" can be used to override the
  default, which is 10. The possible values are 10 (LF), 13 (CR),
  3338 (CRLF), -1 (ANY), or -2 (ANYCRLF).])

if test "$enable_bsr_anycrlf" = "yes"; then
  AC_DEFINE([BSR_ANYCRLF], [], [
    By default, the \R escape sequence matches any Unicode line ending
    character or sequence of characters. If BSR_ANYCRLF is defined, this is
    changed so that backslash-R matches only CR, LF, or CRLF. The build-
    time default can be overridden by the user of PCRE at runtime. On
    systems that support it, "configure" can be used to override the
    default.])
fi

AC_DEFINE_UNQUOTED([LINK_SIZE], [$with_link_size], [
  The value of LINK_SIZE determines the number of bytes used to store
  links as offsets within the compiled regex. The default is 2, which
  allows for compiled patterns up to 64K long. This covers the vast
  majority of cases. However, PCRE can also be compiled to use 3 or 4
  bytes instead. This allows for longer patterns in extreme cases. On
  systems that support it, "configure" can be used to override this default.])

AC_DEFINE_UNQUOTED([POSIX_MALLOC_THRESHOLD], [$with_posix_malloc_threshold], [
  When calling PCRE via the POSIX interface, additional working storage
  is required for holding the pointers to capturing substrings because
  PCRE requires three integers per substring, whereas the POSIX
  interface provides only two. If the number of expected substrings is
  small, the wrapper function uses space on the stack, because this is
  faster than using malloc() for each call. The threshold above which
  the stack is no longer used is defined by POSIX_MALLOC_THRESHOLD. On
  systems that support it, "configure" can be used to override this
  default.])

AC_DEFINE_UNQUOTED([MATCH_LIMIT], [$with_match_limit], [
  The value of MATCH_LIMIT determines the default number of times the
  internal match() function can be called during a single execution of
  pcre_exec(). There is a runtime interface for setting a different
  limit. The limit exists in order to catch runaway regular
  expressions that take for ever to determine that they do not match.
  The default is set very large so that it does not accidentally catch
  legitimate cases. On systems that support it, "configure" can be
  used to override this default default.])

AC_DEFINE_UNQUOTED([MATCH_LIMIT_RECURSION], [$with_match_limit_recursion], [
  The above limit applies to all calls of match(), whether or not they
  increase the recursion depth. In some environments it is desirable
  to limit the depth of recursive calls of match() more strictly, in
  order to restrict the maximum amount of stack (or heap, if
  NO_RECURSE is defined) that is used. The value of
  MATCH_LIMIT_RECURSION applies only to recursive calls of match(). To
  have any useful effect, it must be less than the value of
  MATCH_LIMIT. The default is to use the same value as MATCH_LIMIT.
  There is a runtime method for setting a different limit. On systems
  that support it, "configure" can be used to override the default.])

AC_DEFINE([MAX_NAME_SIZE], [32], [
  This limit is parameterized just in case anybody ever wants to
  change it. Care must be taken if it is increased, because it guards
  against integer overflow caused by enormously large patterns.])

AC_DEFINE([MAX_NAME_COUNT], [10000], [
  This limit is parameterized just in case anybody ever wants to
  change it. Care must be taken if it is increased, because it guards
  against integer overflow caused by enormously large patterns.])

AH_VERBATIM([PCRE_EXP_DEFN], [
/* If you are compiling for a system other than a Unix-like system or
   Win32, and it needs some magic to be inserted before the definition
   of a function that is exported by the library, define this macro to
   contain the relevant magic. If you do not define this macro, it
   defaults to "extern" for a C compiler and "extern C" for a C++
   compiler on non-Win32 systems. This macro apears at the start of
   every exported function that is part of the external API. It does
   not appear on functions that are "external" in the C sense, but
   which are internal to the library. */
#undef PCRE_EXP_DEFN])

# Platform specific issues
NO_UNDEFINED=
EXPORT_ALL_SYMBOLS=

# The extra LDFLAGS for each particular library
# (Note: The libpcre*_version bits are m4 variables, assigned above)

EXTRA_LIBPCRE_LDFLAGS="$NO_UNDEFINED -version-info libpcre_version"

AC_SUBST(EXTRA_LIBPCRE_LDFLAGS)

# Produce these files, in addition to config.h.
AC_CONFIG_FILES([Makefile])

AC_OUTPUT


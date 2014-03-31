#if defined(CPU_ARM)
#else
/* Define backtrace type */
#define BACKTRACE_METHOD_TEXT "x86 custom backtrace"
/* Define backtrace type */
#define BACKTRACE_METHOD_X86_LINUX 1
#endif

// Bogus in the end, as the backtrace method for unix is x86-only.
#define BACKTRACE_DEMANGLE_METHOD_TEXT "cxa_demangle"
#define BACKTRACE_LOOKUP_METHOD_DLADDR 1
#define BACKTRACE_LOOKUP_METHOD_TEXT "dladdr"

#define UNIX 1
#define LINUX 1
#define LUA_USE_LINUX 1
#define STDC_HEADERS 1

#define ENDIAN_LITTLE 1
#define HAVE_ALLOCA_H 1
#define HAVE_DECL_SIGPWR 1
#define HAVE_DECL_SIGUSR1 1
#define HAVE_ENDIAN_H 1
#define HAVE_FFMPEG 1
#define HAVE_INTTYPES_H 1
#define HAVE_LIBPTHREAD 1
#define HAVE_LIBXTST 1
#define HAVE_MEMORY_H 1
#define HAVE_PTHREAD_COND_TIMEDWAIT 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_TLS 1
#define HAVE_UNISTD_H 1
#define ICONV_CONST
#define _GNU_SOURCE 1
#define __STDC_FORMAT_MACROS 1
#define thread_local __thread
#define SUPPORT_GLES2_REGAL
#define SUPPORT_GLES2


// Stepmania Bug Reporting
#define PACKAGE_BUGREPORT "http://ssc.ajworld.net/sm-ssc/"
#define PACKAGE_NAME "StepMania 5"
#define PACKAGE_STRING "StepMania 5 alpha 0 variant Android"
#define PACKAGE_TARNAME "StepMania"
#define PACKAGE_URL ""
#define PACKAGE_VERSION "alpha 0 variant Android"


#define HAVE_LOADINGWINDOW

/* OpenGL ES 2.0 support not available */
/* #undef WITHOUT_GLES2 */

/* For now, comment out Networking on Android */
//#define WITHOUT_NETWORKING

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

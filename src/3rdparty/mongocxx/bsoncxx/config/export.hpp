
#ifndef BSONCXX_API_H
#define BSONCXX_API_H

#ifdef BSONCXX_STATIC
#  define BSONCXX_API
#  define BSONCXX_PRLCTTE
#else
#  ifndef BSONCXX_API
#    ifdef BSONCXX_EXPORT
        /* We are building this library */
#      define BSONCXX_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define BSONCXX_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef BSONCXX_PRLCTTE
#    define BSONCXX_PRLCTTE __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef BSONCXX_DEPRECATED
#  define BSONCXX_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef BSONCXX_DEPRECATED_EXPORT
#  define BSONCXX_DEPRECATED_EXPORT BSONCXX_API BSONCXX_DEPRECATED
#endif

#ifndef BSONCXX_DEPRECATED_NO_EXPORT
#  define BSONCXX_DEPRECATED_NO_EXPORT BSONCXX_PRLCTTE BSONCXX_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef BSONCXX_NO_DEPRECATED
#    define BSONCXX_NO_DEPRECATED
#  endif
#endif

#endif

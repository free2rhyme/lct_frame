
#ifndef MONGOCXX_API_H
#define MONGOCXX_API_H

#ifdef MONGOCXX_STATIC
#  define MONGOCXX_API
#  define MONGOCXX_PRLCTTE
#else
#  ifndef MONGOCXX_API
#    ifdef MONGOCXX_EXPORTS
        /* We are building this library */
#      define MONGOCXX_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define MONGOCXX_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef MONGOCXX_PRLCTTE
#    define MONGOCXX_PRLCTTE __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef MONGOCXX_DEPRECATED
#  define MONGOCXX_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef MONGOCXX_DEPRECATED_EXPORT
#  define MONGOCXX_DEPRECATED_EXPORT MONGOCXX_API MONGOCXX_DEPRECATED
#endif

#ifndef MONGOCXX_DEPRECATED_NO_EXPORT
#  define MONGOCXX_DEPRECATED_NO_EXPORT MONGOCXX_PRLCTTE MONGOCXX_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef MONGOCXX_NO_DEPRECATED
#    define MONGOCXX_NO_DEPRECATED
#  endif
#endif

#endif

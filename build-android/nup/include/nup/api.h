
#ifndef NUP_API_H
#define NUP_API_H

#ifdef NUP_STATIC_DEFINE
#  define NUP_API
#  define NUP_NO_EXPORT
#else
#  ifndef NUP_API
#    ifdef nup_EXPORTS
        /* We are building this library */
#      define NUP_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define NUP_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef NUP_NO_EXPORT
#    define NUP_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef DEPRECATED_API
#  define DEPRECATED_API __attribute__ ((__deprecated__))
#endif

#ifndef DEPRECATED_API_EXPORT
#  define DEPRECATED_API_EXPORT NUP_API DEPRECATED_API
#endif

#ifndef DEPRECATED_API_NO_EXPORT
#  define DEPRECATED_API_NO_EXPORT NUP_NO_EXPORT DEPRECATED_API
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef NUP_NO_DEPRECATED
#    define NUP_NO_DEPRECATED
#  endif
#endif

#endif /* NUP_API_H */

#ifndef _COMMON_H
#define _COMMON_H

#ifdef CC_PRINT
#include <stdio.h>
#endif
#include <string.h>

#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#define GCC_VERSION 0
#endif

/* Are two vars/types the same type (ignoring qualitfiers)?*/
#define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))

#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:(-!!(e)); }))

#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

/* define __attribute__ ((*)) */
#if GCC_VERSION >= 30400
#undef inline
#define inline         inline __attribute__ ((always_inline))
#define __noinline      __attribute__ ((noinline))
#define __pure          __attribute__ ((pure))
#define __const         __attribute__ ((const))
#define __noreturn      __attribute__ ((noreturn))
#define __malloc        __attribute__ ((malloc))
#define __must_check    __attribute__ ((warn_unused_result))
#define __deprecated    __attribute__ ((deprecated))
#define __used          __attribute__ ((used))
#define __unused        __attribute__ ((unused))
#define __packed        __attribute__ ((packed))
#define __align(x)      __attribute__ ((aligned, (x)))
#define __align_max     __attribute__ ((aligned))
#define __init          __attribute__ ((constructor))
#define __exit          __attribute__ ((destructor))
#define likely(x)       __builtin_expect (!!(x), 1)
#define unlikely(x)     __builtin_expect (!!(x), 0)
#else
#undef inline
#define __noinline   /* no noinline           */
#define __pure       /* no pure               */
#define __const      /* no const              */
#define __noreturn   /* no noreturn           */
#define __malloc     /* no malloc             */
#define __must_check /* no warn_unused_result */
#define __deprecated /* no deprecated         */
#define __used       /* no used               */
#define __unused     /* no unused             */
#define __packed     /* no packed             */
#define __align(x)   /* no aligned            */
#define __align_max  /* no align_max          */
#define __init
#define __exit
#define likely(x)    (x)
#define unlikely(x)  (x)
#endif

#if defined(TRUE) || defined(FALSE)
#undef TRUE
#undef FALSE
#endif
#define TRUE 1
#define FALSE !TRUE

/*
 * count number of elements of an array,
 * it should be a compile error if passing a pointer as an argument
 */
#define ARRAY_COUNT(x) ((sizeof (x) / sizeof (0[x])) \
                        / ((size_t) !(sizeof (x) % sizeof(0[x]))))

/* some useful string macros */
#define STRING_EQUAL(s1, s2)  (strcmp(s1, s2) == 0)
#define STRING_CONTAIN(s, ss) (strstr(s, ss) != NULL)
#define STRING_EMPTY(s1)      (strlen(s1) == 0)

#ifdef CC_PRINT
#if CC_PRINT_LEVEL == 1
#define CC_INFO(format, ...)
#define CC_WARNING(format, ...)
#define CC_ERROR(format, ...) \
    fprintf(stderr, "error: %s:%s():%d: " format, \
            __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#elif CC_PRINT_LEVEL == 2
#define CC_INFO(format, ...)
#define CC_WARNING(format, ...) \
    fprintf(stdout, "warning: %s:%s():%d: " format, \
            __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define CC_ERROR(format, ...) \
    fprintf(stderr, "error: %s:%s():%d: " format, \
            __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#elif CC_PRINT_LEVEL == 3
#define CC_INFO(format, ...) \
    fprintf(stdout, "info: %s:%s():%d: " format, \
            __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define CC_WARNING(format, ...) \
    fprintf(stdout, "warning: %s:%s():%d: " format, \
            __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define CC_ERROR(format, ...) \
    fprintf(stderr, "error: %s:%s():%d: " format, \
            __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#else
#error Sorry, you defined an unsupported CC_PRINT_LEVEL
#endif /* CC_PRINT_LEVEL == 2 */
#else
#define CC_INFO(format, ...)
#define CC_WARNING(format, ...)
#define CC_ERROR(format, ...)
#endif /* CC_PRINT */

/* lambda */
#define lambda(return_type, ...) \
    __extension__ \
    ({ \
        return_type __fn__ __VA_ARGS__ \
        __fn__; \
    })

#endif /* _COMMON_H */


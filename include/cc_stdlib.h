/*
 * ISO C standard: general utilities <cc_stdlib.h>
 */
#ifndef _CC_STDLIB_H
#define _CC_STDLIB_H

#define __need_size_t /* get size_t from <cc_stddef.h> */
#include <cc_stddef.h>
#include <cc_string.h>

#define NALLOC 1024 /* minimum units to request */

typedef long Align; /* for alignment to long boundary */

union header { /* block header */
    struct {
        union header *ptr; /* next block if on free list */
        unsigned size;     /* size of this block */
    } s;
    Align x; /* force alignment of blocks */
};
typedef union header Header;

extern void *cc_malloc(size_t size);
extern void *cc_calloc(size_t nmem, size_t size);
extern void *cc_realloc(void *ptr, size_t size);
extern void cc_free(void *ap);

#define SWAP_TY(__x, __y, __t) do { \
    __t tmp; tmp = __x; __x = __y; __y = tmp; \
} while (0)

#define SWAP_MEM(__x, __y, __s) do { \
    char tmp[(__s)]; \
    cc_memcpy(tmp, &__x, __s); \
    cc_memcpy(&__x, &__y, __s); \
    cc_memcpy(&__y, tmp, __s); \
} while (0)

void cc_qsort(void *base, size_t nmemb, size_t size,
              int (*compare)(const void *, const void *));
void *cc_bsearch(const void *key, const void *base, size_t nmemb, size_t size,
                 int (*compare)(const void *key, const void *elt));

#endif /* !_CC_STDLIB_H */


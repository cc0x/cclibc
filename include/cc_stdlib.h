/*
 * ISO C standard: general utilities <cc_stdlib.h>
 */
#ifndef _CC_STDLIB_H
#define _CC_STDLIB_H 1

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

/* cc_qsort: sort an array */
void cc_qsort(void *base, size_t nmemb, size_t size,
			  int (*compare)(const void *, const void *));
/* cc_bsearch: an implement of binary search algorithm */
void *cc_bsearch(const void *key, const void *base, size_t nmemb, size_t size,
				 int (*compare)(const void *key, const void *elt));

/*
 * vector_t type
 */
#define NEW_VECTOR_SIZE 16

typedef struct {
	void **items;
	int capacity;
	int size;
} vector_t;

extern vector_t *vector_new(void);
extern int vector_size(vector_t *v);
extern void *vector_at(vector_t *v, int index);
extern int vector_push(vector_t *v, void *item);
extern int vector_insert(vector_t *v, int index, void *item);
extern void *vector_pop(vector_t *v);
extern int vector_delete(vector_t *v, int index);
extern int vector_find(vector_t *v, void *item);

#endif

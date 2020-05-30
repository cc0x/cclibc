#include <unistd.h>

#include <cc_stdlib.h>
#include <cc_string.h>
#include <cc_stddef.h>

static Header base;    /* empty list to get started */
static Header *freep;  /* start of free list */

static Header *morecore(unsigned nu)
{
    void *cp;
    Header *up;

    if (nu < NALLOC)
        nu = NALLOC;

    cp = sbrk(sizeof(Header) * nu);
    if (cp == (void *)-1)
        return NULL;
    up = (Header *) cp;
    up->s.size = nu;
    cc_free((void *)(up + 1));

    return freep;
}

void *cc_malloc(size_t size)
{
    Header *p, *prevp;
    unsigned nunits;

    nunits = (size + sizeof(Header) - 1) / sizeof(Header) + 1;
    if (nunits == 1) /* size is zero */
        return NULL;

    if (!(prevp = freep)) { /* no free list yet */
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
        if (p->s.size >= nunits) { /* big enough */
            if (p->s.size == nunits) { /* exactly */
                prevp->s.ptr = p->s.ptr;
            } else { /* allocate tail end */
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void *)(p + 1);
        }
        if (p == freep) /* wrapped around free list */
            if ((p = morecore(nunits)) == NULL)
                return NULL; /* none left */
    }
}

void *cc_calloc(size_t nmem, size_t size)
{
    void *data;

    data = cc_malloc(nmem * size); /* allocates nmem * size bytes by calling cc_malloc */
    if (!data)
        cc_memset(data, 0, nmem * size); /* set memory area to zero */

    return data;
}

void *cc_realloc(void *p, size_t size)
{
    size_t pre_size;
    void *tmp;

    pre_size = (((Header *)p - 1)->s.size - 1) * sizeof(Header);
    cc_free(p);

    tmp = cc_malloc(size);
    if (tmp)
        cc_memcpy(tmp, p, size <= pre_size ? size : pre_size);

    return tmp;
}

void cc_free(void *ap)
{
    Header *bp, *p;

    bp = (Header *)ap - 1; /* point to block header */
    if (bp->s.size == 0) /* free block is zero in size */
        return;

    for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
        if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
            break; /* free block at start or end of arena */

    if (bp + bp->s.size == p->s.ptr) { /* join to upper nbr */
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else {
        bp->s.ptr = p->s.ptr;
    }

    if (p + p->s.size == bp) { /* join to lower nbr */
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }

    freep = p;
}

static void *partition(void *base, size_t nmemb, size_t size,
                       int (*compare)(const void *key, const void *elt))
{
    char *pivot;
    char *head, *tail;

    if (nmemb <= 1)
        return NULL;

    pivot = base + (nmemb - 1) * size;

    head = base;
    tail = pivot - size;
    while (1) {
        for(; head <= tail && compare(head, pivot) <= 0; head += size);
        for(; tail >= head && compare(tail, pivot) >= 0; tail -= size);

        if (head >= tail)
            break;

        SWAP_MEM(head, tail, size);
    }

    SWAP_MEM(head, pivot, size);

    return head;
}

void cc_qsort(void *base, size_t nmemb, size_t size,
              int (*compare)(const void *key, const void *elt))
{
    char *pivot;

    pivot = partition(base, nmemb, size, compare);

    if (pivot == NULL)
        return;

    cc_qsort(base, (pivot - (char *)base) / size, size, compare);
    cc_qsort(pivot + size, nmemb - ((pivot - (char *) base) / size) -1 , size, compare);
}

void *cc_bsearch(const void *key, const void *base, size_t nmemb, size_t size,
                 int (*compare)(const void *key, const void *elt))
{
    const char *pivot;
    int result;

    while (nmemb > 0) {
        pivot = base + (nmemb >> 1) * size;
        result = compare(key, pivot);

        if (result == 0)
            return (void *)pivot;

        if (result > 0) {
            base = pivot + size;
            nmemb--;
        }
        nmemb >>= 1;
    }

    return NULL;
}

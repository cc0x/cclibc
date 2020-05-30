#include <unistd.h>

#include <cc_stdlib.h>
#include <cc_string.h>
#include <cc_stddef.h>

static Header base;    /* empty list to get started */
static Header *freep;  /* start of free list */

/*
 * @rief Ask system for more memory
 */
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

/*
 * @brief General-purpose storage allocator
 */
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

/*
 * @brief Allocates memory for an array of nmem elements of size bytes each
 */
void *cc_calloc(size_t nmem, size_t size)
{
    void *data;

    data = cc_malloc(nmem * size); /* allocates nmem * size bytes by calling cc_malloc */
    if (!data)
        cc_memset(data, 0, nmem * size); /* set memory area to zero */

    return data;
}

/*
 * cc_bfree: frees an arbitrary block p of n bytes into the freelist
 * maintained by cc_malloc and cc_free
 */
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

/*
 * @brief Put block ap in free list
 */
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

/*
 * @brief An implement of binary search algorithm
 *
 * @param[in] key Search key
 * @param[in] base Pointer to initial member of the searched array,
 * that should be in ascending sorted order acc_ording to the
 * comparison function
 * @param[in] nmemb Number objects of the searched array
 * @param[in] size The size of each member of the searched array
 * @param[in] compare A compare function
 *
 * @return a pointer to a matching member of the array, or NULL if no match
 * is found.
 */
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

/*
 * vector_t implement
 */
static int vector_resize(vector_t *v, int cap)
{
    void **tmp = cc_realloc(v->items, (sizeof *v->items) * cap);
    if (!tmp)
        return -1;

    v->items = tmp;
    v->capacity = cap;

    return 0;
}

vector_t *vector_new(void)
{
    vector_t *v;

    v = cc_malloc(sizeof(vector_t));
    if (!v)
        return NULL;

    v->items = cc_malloc((sizeof *v->items) * NEW_VECTOR_SIZE);
    if (!v->items) {
        cc_free(v);
        return NULL;
    }
    v->capacity = NEW_VECTOR_SIZE;
    v->size = 0;

    return v;
}

int vector_size(vector_t *v)
{
    return v->size;
}

void *vector_at(vector_t *v, int index)
{
    if (index >= v->size || index < 0)
        return NULL;

    return *(v->items + index);
}

int vector_push(vector_t *v, void *item)
{
    if (v->size == v->capacity)
        if (vector_resize(v, v->capacity * 2))
            return -1;

    *(v->items + v->size++) = item;

    return 0;
}

int vector_insert(vector_t *v, int index, void *item)
{
    if (index >= v->size || index < 0)
        return -1;

    if (v->size == v->capacity)
        if (vector_resize(v, v->capacity * 2))
            return -1;

    for (int i = index; i < v->size; i++)
        *(v->items + i + 1) = *(v->items + i);
    *(v->items + index) = item;

    return 0;
}

void *vector_pop(vector_t *v)
{
    return *(v->items + --v->size);
}

int vector_delete(vector_t *v, int index)
{
    if (index >= v->size || index < 0)
        return -1;

    for (int i = index; i < v->size; i++)
        *(v->items + i) = *(v->items + i + 1);
    v->size--;

    return 0;
}

int vector_find(vector_t *v, void *item)
{
    for (int i = 0; i < v->size; i++)
        if (*(v->items + i) == item)
            return i;

    return -1;
}


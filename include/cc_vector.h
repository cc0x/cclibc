#ifndef _CC_VECTOR_H
#define _CC_VECTOR_H

#include "cc_stdlib.h"

typedef struct _vector vector_t;

struct _vector{
    struct {
        void *items;
        size_t data_size;
        size_t capacity;
        size_t size;
    } _attr;

    size_t (*size)(vector_t *self);
    size_t (*capacity)(vector_t *self);
    void *(*at)(vector_t *self, size_t index);
    void *(*push)(vector_t *self, void *item);
    void *(*insert)(vector_t *self, size_t index, void *item);
    void *(*pop)(vector_t *self);
    void *(*remove)(vector_t *self, size_t index);
    void *(*find)(vector_t *self, void *item, int (*compare)(void *item1, void *item2));
};

#define NEW_VECTOR_SIZE 2

#define INIT_VECTOR(var, type)                                      \
do {                                                                \
    void *items = cc_malloc(sizeof(type) * NEW_VECTOR_SIZE);        \
    vector_t (__var) = {                                            \
        ._attr    = {                                               \
            .items     = items,                                     \
            .data_size = sizeof(type),                              \
            .capacity  = NEW_VECTOR_SIZE,                           \
            .size      = 0,                                         \
        },                                                          \
        .size     = _vector_size,                                   \
        .capacity = _vector_capacity,                               \
        .at       = _vector_at,                                     \
        .push     = _vector_push,                                   \
        .insert   = _vector_insert,                                 \
        .pop      = _vector_pop,                                    \
        .remove   = _vector_remove,                                 \
        .find     = _vector_find,                                   \
    };                                                              \
    (var) = __var;                                                  \
} while (0)

#define DES_VECTOR(var)                                             \
do {                                                                \
    cc_free((var)._attr.items);                                     \
} while (0)

extern size_t _vector_size(vector_t *self);
extern size_t _vector_capacity(vector_t *self);
extern void *_vector_at(vector_t *self, size_t index);
extern void *_vector_push(vector_t *self, void *item);
extern void *_vector_insert(vector_t *self, size_t index, void *item);
extern void *_vector_pop(vector_t *self);
extern void *_vector_find(vector_t *self, void *item, int (*compare)(void *item1, void *item2));
extern void *_vector_remove(vector_t *self, size_t index);

#endif /* _CC_VECTOR_H */
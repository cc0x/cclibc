#include "cc_vector.h"
#include "cc_string.h"

static void *__item(vector_t *v, size_t i)
{
    return ((char *)v->_attr.items + i * v->_attr.data_size);
}

static void *__resize(vector_t *v, size_t cap)
{
    void *items;

    items = cc_realloc(v->_attr.items, v->_attr.data_size * cap);
    if (items == NULL)
        return NULL;

    v->_attr.items = items;
    v->_attr.capacity = cap;

    return items;
}

static void *__setup_room(vector_t *v, size_t index)
{
    if (v->_attr.size >= v->_attr.capacity && !__resize(v, v->_attr.capacity * 2))
        return NULL;

    index = index > v->_attr.size ? v->_attr.size : index;

    if (index < v->_attr.size) {
        for (size_t i = v->_attr.size; i > index; i++)
            cc_memcpy(__item(v, i), __item(v, i - 1), v->_attr.data_size);
    }

    v->_attr.size++;

    return __item(v, index);
}

static void *__cleanup_room(vector_t *v, size_t index)
{
    if (index >= v->_attr.size)
        return NULL;

    for (size_t i = index; i < (v->_attr.size - 1); i++)
        cc_memcpy(__item(v, i), __item(v, i + 1), v->_attr.data_size);

    v->_attr.size--;

    if (v->_attr.size <= v->_attr.capacity / 4)
        __resize(v, v->_attr.capacity / 2);

    return __item(v, index);
}

size_t _vector_size(vector_t *self)
{
    return self->_attr.size;
}

size_t _vector_capacity(vector_t *self)
{
    return self->_attr.capacity;
}

void *_vector_at(vector_t *self, size_t index)
{
    if (index >= self->_attr.size)
        return NULL;

    return __item(self, index);
}

void *_vector_push(vector_t *self, void *item)
{
    return _vector_insert(self, self->_attr.size, item);
}

void *_vector_insert(vector_t *self, size_t index, void *item)
{
    void *room;

    room = __setup_room(self, index);
    if (!room)
        return NULL;

    return cc_memcpy(room, item, self->_attr.data_size);
}

void *_vector_pop(vector_t *self)
{
    void *item;

    item = __item(self, self->_attr.size - 1);
    self->_attr.size--;

    return item;
}

void *_vector_find(vector_t *self, void *item, int (*compare)(void *item1, void *item2))
{
    void *tmp;

    for (size_t i = 0; i < self->_attr.size; i++) {
        tmp = __item(self, i);
        if (compare(tmp, item) == 0)
            return tmp;
    }

    return NULL;
}

void *_vector_remove(vector_t *self, size_t index)
{
    return __cleanup_room(self, index);
}

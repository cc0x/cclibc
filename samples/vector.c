#include <stdio.h>
#include <cc_vector.h>

int compare(int *item1, int *item2)
{
    return !(*item1 == *item2);
}

int main(void)
{
    vector_t vec;

    INIT_VECTOR(vec, int);
    int n = 101010;
    int f = 25;

    for (int i = 0; i < 100; i++)
        vec.push(&vec, &i);

    vec.insert(&vec, 10, &n);
    vec.remove(&vec, 20);

    for (int i = 0; i < 100; i++)
        printf("%i\n", *(int *)vec.pop(&vec));

    printf("Find %i: %i\n", f, *(int *)vec.find(&vec, &f, (int (*)(void *, void *))compare));

    DES_VECTOR(vec);

    return 0;
}

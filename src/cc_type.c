#include <cc_type.h>
#define __need_boolean /* get TRUE/FALSE from <cc_stddef.h> */
#include <cc_stddef.h>

int cc_isspace(char c)
{
    return c == ' ' || c == '\t' ? 1 : 0;
}

int cc_isdigit(int src)
{
    if (src > '0' && src < '9')
        return TRUE;
    return FALSE;
}


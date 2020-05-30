#ifndef _CC_STRING_H
#define _CC_STRING_H

/* get size_t and NULL from <cc_stddef.h> */
#define __need_size_t
#define __need_NULL
#include <cc_stddef.h>

extern int cc_atoi(const char *s);
extern double cc_atof(const char *s);
extern int cc_htoi(const char *s);

extern size_t cc_strlen(const char *src);
extern char *cc_strrev(char *dest, const char *src);
extern char *cc_strsqueeze(char *s1, const char *s2);
extern int cc_tolower(int src);
extern int cc_toupper(int src);
extern int cc_strindex(const char *src, const char *dest);
extern char *cc_strcat(char *dest, const char *src);
extern char *cc_strncat(char *dest, const char *src, size_t n);
extern char *cc_trim(char *s);
extern char *cc_strcpy(char *dest, const char *src);
extern char *cc_strncpy(char *dest, const char *src, size_t n);
extern int cc_strcmp(char *s1, const char *s2);
extern int cc_strcasecmp(const char *s1, const char *s2);

extern void *cc_memcpy(void *dest, const void *src, size_t n);
extern void *cc_memset(void *s, int c, size_t n);

#endif /* !_CC_STRING_H */


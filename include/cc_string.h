/*
 * ISO C standard: string handling <cc_string.h>
 */
#ifndef _CC_STRING_H
#define _CC_STRING_H 1

/* get size_t and NULL from <cc_stddef.h> */
#define __need_size_t
#define __need_NULL
#include <cc_stddef.h>

/* cc_atoi: convert a string to integer */
extern int cc_atoi(const char *s);
/* cc_atof: convert a string to float */
extern double cc_atof(const char *s);
/* cc_htoi: convert a string of hexadecimal digits into integer */
extern int cc_htoi(const char *s);

/* cc_strlen: calculate the length of a string */
extern size_t cc_strlen(const char *src);
/* cc_strrev: reverse a string */
extern char *cc_strrev(char *dest, const char *src);
/* cc_strsqueeze: delete each character in s1 that match any character in s2 */
extern char *cc_strsqueeze(char *s1, const char *s2);
/* cc_tolower: convert a string to lower case */
extern int cc_tolower(int src);
/* cc_toupper: convert a string to upper case */
extern int cc_toupper(int src);
/* cc_strindex: return the index of string dest in string src */
extern int cc_strindex(const char *src, const char *dest);
/* cc_strcat: concatenate src string to the end of dest */
extern char *cc_strcat(char *dest, const char *src);
/* cc_strncat: concatenate at most n bytes of src string to dest */
extern char *cc_strncat(char *dest, const char *src, size_t n);
/* cc_trim: remove leading/trailing white space */
extern char *cc_trim(char *s);
/* cc_strcpy: copy src string to dest */
extern char *cc_strcpy(char *dest, const char *src);
/* cc_strcpy: copy at most n bytes of src string to dest */
extern char *cc_strncpy(char *dest, const char *src, size_t n);
/* cc_strcmp: simple strcmp libc version */
extern int cc_strcmp(char *s1, const char *s2);
/* cc_strcasecmp: a simple strcasecmp libc version */
extern int cc_strcasecmp(const char *s1, const char *s2);

extern void *cc_memcpy(void *dest, const void *src, size_t n);
extern void *cc_memset(void *s, int c, size_t n);

#endif

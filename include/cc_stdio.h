/*
 * ISO C standard: Input/Output <cc_stdio.h>
 */
#ifndef _CC_STDIO_H
#define _CC_STDIO_H

#define __need_size_t /* get size_t from <cc_stddef.h> */
#include <cc_stddef.h>

#ifdef NULL
#undef NULL
#endif
#define NULL 0
#define EOF (-1)
#define OPEN_MAX 20 /* max #files open at once */
#define BUFFSIZE 1024

typedef struct __iobuf {
    int cnt;     /* characters left */
    char *base;  /* location of buffer */
    char *ptr;   /* next character position */
    int flag;    /* mode of file access */
    int fd;      /* file descriptor */
} CC_FILE;

extern CC_FILE __iob[OPEN_MAX];

#define cc_stdin  (&__iob[0]) /* standard input */
#define cc_stdout (&__iob[1]) /* standard output */
#define cc_stderr (&__iob[2]) /* standard error */

enum __flags {
    __READ   = 01,   /* file open for reading */
    __WRITE  = 02,   /* file open for writing */
    __UNBUF  = 04,   /* file is unbuffered */
    __EOF    = 010,  /* EOF has occurred on this file */
    __ERR    = 020,  /* error occurred on this file */
};

#define cc_feof(p)   (((p)->flag & __EOF) != 0)
#define cc_ferror(p) (((p)->flag & __ERR) != 0)
#define cc_fileno(p) ((p)->fd)

#define cc_getc(p)     (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : __fillbuf(p))
#define cc_putc(x, p)  (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : __flushbuf((x), p))
#define cc_getchar()   cc_getc(cc_stdin)
#define cc_putchar(x)  cc_putc((x), cc_stdout)

extern int __fillbuf(CC_FILE *__fp);
extern int __flushbuf(int x, CC_FILE *__fp);

extern CC_FILE *cc_fopen(const char *name, const char *mode);
extern int cc_fflush(CC_FILE *fp);
extern int cc_fclose(CC_FILE *fp);
extern int cc_getline(CC_FILE *fp, char *dest, size_t n);
extern int cc_getfloat(float *fp);
extern int cc_getch(void);
extern void cc_ungetch(int c);
extern void cc_fprintf(CC_FILE *fp, const char *s);

#endif /* !_CC_STDIO_H */


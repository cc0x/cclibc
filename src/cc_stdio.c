#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <cc_stdio.h>
#include <cc_stdlib.h>
#include <cc_type.h>

#define PERM 0666

CC_FILE __iob[OPEN_MAX] = { /* cc_stdin, cc_stdout, and cc_stderr */
    {0, (char *) 0, (char *) 0, __READ, 0},
    {0, (char *) 0, (char *) 0, __WRITE, 1},
    {0, (char *) 0, (char *) 0, __WRITE | __UNBUF, 2}
};

static int __bufp;
static char __buf[BUFFSIZE];

int __fillbuf(CC_FILE *fp)
{
    size_t bufsize;

    if ((fp->flag & (__READ | __EOF | __ERR)) != __READ)
        return __EOF;

    bufsize = (fp->flag & __UNBUF) ? 1 : BUFFSIZE;
    if (fp->base == NULL)
        if ((fp->base = cc_malloc(bufsize)) == NULL)
            return EOF;
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0) {
        if (fp->cnt == -1)
            fp->flag |= __EOF;
        else
            fp->flag |= __ERR;
        fp->cnt = 0;
        return EOF;
    }

    return (unsigned char) *fp->ptr++;
}

int __flushbuf(int x, CC_FILE *fp)
{
    int bufsize;

    if ((fp->flag & (__WRITE | __ERR)) != __WRITE)
        return __ERR;

    bufsize = (fp->flag & __UNBUF) ? 1 : BUFFSIZE;
    if (fp->base == NULL) {
        if ((fp->base = cc_malloc(bufsize)) == NULL)
            return EOF;
    } else {
        if (write(fp->fd, fp->base, bufsize) != bufsize) {
            fp->flag |= __ERR;
            return EOF;
        }
    }
    fp->ptr = fp->base;
    fp->cnt = bufsize - 1;

    return (unsigned char) (*fp->ptr = x);
}

CC_FILE *cc_fopen(const char *name, const char *mode)
{
    int fd;
    CC_FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;

    for (fp = __iob; fp < __iob + OPEN_MAX; fp++)
        if ((fp->flag & (__READ | __WRITE)) == 0) /* found free slot */
            break;
    if (fp >= __iob + OPEN_MAX) /* no free slot */
        return NULL;

    if (*mode == 'w') {
        fd = creat(name, PERM);
    } else if (*mode == 'a') {
        if ((fd = open(name, O_RDONLY, 0)) == -1) /* no such file */
            fd = creat(name, PERM);
        lseek(fd, 0, SEEK_CUR);
    } else if (*mode == 'r') {
        fd = open(name, O_RDONLY, 0);
    } else {
        return NULL;
    }

    if (fd == -1)
        return NULL;
    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flag = (*mode == 'r') ? __READ : __WRITE;

    return fp;
}

int cc_fflush(CC_FILE *fp)
{
    int bufsize, wb;

    if ((fp->flag & (__WRITE | __ERR)) != __WRITE)
        return __ERR;

    if (fp->base == NULL)
        return EOF;

    bufsize = (fp->flag & __UNBUF) ? 1 : BUFFSIZE;
    wb = bufsize - fp->cnt;
    if (write(fp->fd, fp->base, wb) != wb) {
        fp->flag |= __ERR;
        return EOF;
    }
    fp->ptr = fp->base;
    fp->cnt = bufsize;

    return 0;
}

int cc_fclose(CC_FILE *fp)
{
    if (fp->flag & __WRITE)
        cc_fflush(fp);

    if (fp->base != NULL)
        cc_free(fp->base);

    return close(fp->fd);
}

int cc_getline(CC_FILE *fp, char *dest, size_t n)
{
    int c;
    size_t i;

    for (i = 0; i < n - 1 && (c = cc_getc(fp)) != EOF && c != '\n'; i++)
        dest[i] = c;
    dest[i] = '\0';

    return i;
}

int cc_getfloat(float *fp)
{
    int c, sign;
    double power;

    while ((c = cc_getch()) == ' ' || c == '\t');
    if (!cc_isdigit(c) && c != '.' && c != '-' && c != '+') {
        cc_ungetch(c);
        return 0;
    }

    sign = (c == '-') ? -1 : 1;
    if (c == '-' || c == '+')
        c = cc_getch();
    *fp = 0.0;
    while (cc_isdigit(c)) {
        *fp = *fp * 10.0 + c - '0';
        c = cc_getch();
    }
    power = 1.0;
    if (c == '.' && cc_isdigit(c = cc_getch()))
        while(cc_isdigit(c)) {
            *fp = *fp * 10 + c - '0';
            power *= 10.0;
            c = cc_getch();
        }
    else
        return c;
    *fp = *fp * sign / power;

    if (c != EOF)
        cc_ungetch(c);

    return c;
}

int cc_getch(void)
{
    return (__bufp > 0) ? __buf[--__bufp] : cc_getchar();
}

void cc_ungetch(int c)
{
    if (__bufp >= BUFFSIZE)
        cc_fprintf(cc_stderr, "ungetch: overflow buffer\n");
    else
        __buf[__bufp++] = c;
}

void cc_fprintf(CC_FILE *fp, const char *s)
{
    write(fp->fd, s, cc_strlen(s));
}


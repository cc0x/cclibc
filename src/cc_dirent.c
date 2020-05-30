#include <sys/stat.h>
#include <sys/dir.h>
#include <fcntl.h>
#include <unistd.h>

#include <cc_stdlib.h>
#include <cc_string.h>
#include <cc_dirent.h>

CCDIR *cc_opendir(const char *name)
{
    int fd;
    struct stat buf;
    CCDIR *dp;

    fd = open(name, O_RDONLY, 0);
    if (fd == -1)
        return NULL;

    if (fstat(fd, &buf) == -1)
        return NULL;

    if (!S_ISDIR(buf.st_mode))
        return NULL;

    dp = cc_malloc(sizeof(CCDIR));
    if (dp == NULL)
        return NULL;

    dp->fd = fd;

    return dp;
}

struct cc_dirent *cc_readdir(CCDIR *dp)
{
    struct direct dir_buf;
    static struct cc_dirent d;

    while (read(dp->fd, (char *)&dir_buf, sizeof dir_buf) == sizeof dir_buf) {
        if (dir_buf.d_ino == 0)
            continue;

        d.ino = dir_buf.d_ino;
        cc_strncpy(d.name, dir_buf.d_name, NAMEMAX);
        d.name[NAMEMAX] = '\0';

        return &d;
    }

    return NULL;
}

int cc_closedir(CCDIR *dp)
{
    if (dp) {
        close(dp->fd);
        cc_free(dp);
    }

    return 0;
}


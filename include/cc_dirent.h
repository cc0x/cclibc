#ifndef _CC_DIRENT_H
#define _CC_DIRENT_H

#define NAMEMAX 14 /* longest filename component; it's system-dependent */

struct cc_dirent {
    long ino;
    char name[NAMEMAX + 1];
};

typedef struct {
    int fd;
    struct cc_dirent dir_ent;
} CCDIR;

extern CCDIR *cc_opendir(const char *name);
extern struct cc_dirent *cc_readdir(CCDIR *dirp);
extern int cc_closedir(CCDIR *dirp);

#endif /* !_CC_DIRENT_H */


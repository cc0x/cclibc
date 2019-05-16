#ifndef _CC_DIRENT_H
#define _CC_DIRENT_H 1

#define NAMEMAX 14 /* longest filename component; */ /* system-dependent */

struct cc_dirent { /* portable directory entry */
	long ino;  /* inode number */
	char name[NAMEMAX + 1];
};

typedef struct { /* minimum DIR: no buffering, etc. */
	int fd;  /* file descriptor */
	struct cc_dirent dir_ent;  /* the directory entry */
} CCDIR;

/* cc_opendir: returns a pointer to a structure called DIR */
extern CCDIR *cc_opendir(const char *name);
/* cc_readdir: read directories in sequence */
extern struct cc_dirent *cc_readdir(CCDIR *dfd);
/* cc_closedir: closes directory file and frees the space */
extern int cc_closedir(CCDIR *dfd);

#endif

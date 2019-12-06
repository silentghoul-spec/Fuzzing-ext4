#include <sys/types.h>
#include <sys/mount.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <sys/syscall.h>

#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	unsigned char v0[8192];
	unsigned char v1[8192];
	char v2[] = ".";
	char v3[] = "lost+found";
	long v4;
	char v5[] = "./7S0W9Rx6";
	char v6[] = "./O2P8IbN4";
	char v7[] = "./ts9NzaK5";
	long v8;
	v4 = syscall(SYS_open, (long)v2, 65536, 0);
	syscall(SYS_getdents64, (long)v4, (long)v1, 2344);
	syscall(SYS_link, (long)v2, (long)v5);
	syscall(SYS_symlink, (long)v5, (long)v6);
	syscall(SYS_chmod, (long)v6, 3072);
	v8 = syscall(SYS_open, (long)v7, 66, 438);
	syscall(SYS_newlstat, (long)v3, (long)v1);
	syscall(SYS_newstat, (long)v5, (long)v1);
	syscall(SYS_lseek, (long)v8, 1325, 2);
	syscall(SYS_readlink, (long)v2, (long)v1, 8192);

	close(v4);
	close(v8);
	return 0;
}
/* Active fds: v4 v8 */
/* Files
Path: .
Type: dir
Xattrs: 
Path: lost+found
Type: dir
Xattrs: 
Path: ./7S0W9Rx6
Type: dir
Xattrs: 
Path: ./O2P8IbN4
Type: symlink
Xattrs: 
Path: ./ts9NzaK5
Type: file
Xattrs: 
*/

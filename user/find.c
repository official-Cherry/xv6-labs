/*
 * Write a simple version of the UNIX find program for xv6: find all the files in a directory tree with a specific name.
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *target)
{
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;
    
    // open directory
    fd = open(path,0);
    if(fd < 0)
    {
        printf("find: cannot open %s\n", path);
        return;
    }

    // read file data
    if(fstat(fd, &st) < 0)
    {
        printf("find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // check file type
    switch(st.type)
    {
        case T_FILE:
            // file name == target
            if(strcmp(path + strlen(path) - strlen(target), target) == 0)
            {   
                printf("%s\n", path);
            }   
            break;

        case T_DIR:
            // traverse items in directory
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
            {
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
        
            while(read(fd, &de, sizeof(de)) == sizeof(de))
            {
                if(de.inum == 0) continue;
                // ignore current&parent directory
                if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;

                // make new path
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;

                // traverse recursively
                find(buf, target);
            }
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) 
{
    if(argc != 3)
    {
        printf("Usage: find <directory> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}


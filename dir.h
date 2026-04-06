#ifndef DIR_H
#define DIR_H

#include <stdint.h>

#define MAX_FILE_NAME 255

typedef enum _INODE_TYPE {
    Regular,
    Directory,
    Symlink // TODO: dont use symlinks yet
} InodeType;

typedef struct { // we dont really need permissions and datablock, were just collecting this
                 // information for printing
    InodeType type;
    uint32_t size;
} Inode;

typedef struct {
    Inode* Inode;
    char Filename[MAX_FILE_NAME + 1]; // + 1 for the null terminator
} File;

typedef struct {
    char* fullPath;
    File* files[];
} CurrentDirectory;

void changeDirectory(char* dir);
void initDir();

#endif

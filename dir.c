#include "dir.h"
#include <dirent.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>

CurrentDirectory* currentDirectory = NULL;

void initDir() {
    currentDirectory->fullPath = getcwd(currentDirectory->fullPath, PATH_MAX);
    int j = 0;
}

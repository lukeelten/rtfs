#ifndef RTFS_RTFS_H
#define RTFS_RTFS_H


#define RTFS_VERSION 1
#define DEFAULT_BLOCKSIZE (1024 * 4)
#define DEFAULT_FILENAME "rtfs.bin"
#define DEFAULT_SIZE (1024 * 1024 * 100)




#include "rtfs/inode.h"
#include "rtfs/superblock.h"
#include "rtfs/formatter.h"
#include "rtfs/instance.h"
#include "rtfs/internal.h"



#endif //RTFS_RTFS_H

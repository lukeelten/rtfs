#ifndef RTFS_RTFS_H
#define RTFS_RTFS_H


#define RTFS_VERSION 0x1
#define DEFAULT_BLOCKSIZE (1024 * 4)
#define DEFAULT_FILENAME "/home/lukeelten/rtfs.bin"
#define DEFAULT_SIZE (1024 * 1024 * 100)




#include "rtfs/inode.h"
#include "rtfs/inode_address.h"
#include "rtfs/superblock.h"
#include "rtfs/formatter.h"
#include "rtfs/instance.h"
#include "rtfs/internal.h"
#include "rtfs/block.h"
#include "rtfs/file.h"
#include "rtfs/folder.h"



#endif //RTFS_RTFS_H

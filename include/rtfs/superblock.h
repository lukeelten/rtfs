//
// Created by lukeelten on 28.11.17.
//

#ifndef RTFS_SUPERBLOCK_H
#define RTFS_SUPERBLOCK_H

#include "rtfs/inode.h"

class Superblock {
    friend class Formatter;

public:
    Superblock() = default;



private:
    u8 version; // RTFS version

    off_t blockSize;
    off_t numInodes;

    off_t totalSize;
    off_t treeSize;
    InodeAddress root;
};


#endif //RTFS_SUPERBLOCK_H

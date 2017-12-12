//
// Created by lukeelten on 28.11.17.
//

#ifndef RTFS_SUPERBLOCK_H
#define RTFS_SUPERBLOCK_H

#include "rtfs/inode.h"

class Superblock {
    friend class Formatter;

public:
    static Superblock readFromDisk();


    u8 getVersion() const noexcept {
        return version;
    }

    off_t getBlockSize() const noexcept {
        return blockSize;
    }

    off_t getNumInodes() const noexcept {
        return numInodes;
    }

    off_t getTotalSize() const noexcept {
        return totalSize;
    }

    off_t getTreeSize() const noexcept {
        return treeSize;
    }

    const InodeAddress& getRoot() const noexcept {
        return root;
    }

private:
    u8 version; // RTFS version

    off_t blockSize;
    off_t numInodes;

    off_t totalSize;
    off_t treeSize;
    InodeAddress root;
};


#endif //RTFS_SUPERBLOCK_H

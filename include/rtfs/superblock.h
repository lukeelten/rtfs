//
// Created by lukeelten on 28.11.17.
//

#ifndef RTFS_SUPERBLOCK_H
#define RTFS_SUPERBLOCK_H

#include "rtfs/inode.h"

class Superblock {
    friend class Formatter;

public:
    Superblock() : blockSize(0), numInodes(0), totalSize(0), treeSize(0), root() {}
    ~Superblock() = default;

    Superblock(const Superblock& ) = default;
    Superblock(Superblock&& ) = default;
    Superblock& operator = (const Superblock& ) = default;
    Superblock& operator = (Superblock&& ) = default;

    static Superblock readFromDisk();

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
    off_t blockSize;
    off_t numInodes;

    off_t totalSize;
    off_t treeSize;
    InodeAddress root;
};


#endif //RTFS_SUPERBLOCK_H

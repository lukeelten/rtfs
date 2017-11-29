//
// Created by lukeelten on 28.11.17.
//

#ifndef RTFS_BTREE_H
#define RTFS_BTREE_H

#include "rtfs/inode.h"

class BTreeKey {
public:
    explicit BTreeKey(unsigned long hash_) : hash(hash_) {}

    unsigned long getFilehash() const noexcept  {
        return hash & 0x00000000FFFFFFFF;
    }

    unsigned long getParenthash() const noexcept  {
        return hash & 0xFFFFFFFF00000000;
    }

    unsigned long getHash() const noexcept  {
        return hash;
    }

private:
    unsigned long hash;
};

class BTreeValue {
private:
    InodeAddress inode;
};

#endif //RTFS_BTREE_H

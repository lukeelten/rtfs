

#ifndef RTFS_BTREE_H
#define RTFS_BTREE_H

#include <string>
#include <list>
#include <memory>
#include <cstdio>
#include <rtfs.h>

#include "rtfs/inode_address.h"

using std::string;
using std::list;
using std::shared_ptr;

class BTree {

public:

    static shared_ptr<BTree> readFromDisk(const FileHandler& file, off_t addr);
    bool save(const FileHandler& handler, off_t addr) const;

    bool exists(const string& path) const noexcept;
    InodeAddress getAddress(const string& path) const;
    list<InodeAddress> getChildren(const string& path) const;

    bool insert(const string& path, InodeAddress addr);
    bool remove(const string& path);

    size_t getSize() const noexcept;

private:
    bool save(FILE* fp, off_t addr);
    friend class Formatter;
};



#endif //RTFS_BTREE_H

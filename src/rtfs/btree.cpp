#include <string>
#include <list>
#include <memory>
#include <cstdio>
#include <rtfs.h>

#include "rtfs/inode_address.h"
#include "rtfs/btree.h"

using namespace std;

std::shared_ptr<BTree> BTree::readFromDisk(const FileHandler &file, off_t addr) {
    return std::shared_ptr<BTree>();
}

bool BTree::save(const FileHandler &handler, off_t addr) const {
    return false;
}

bool BTree::exists(const string &path) const noexcept {
    return false;
}

InodeAddress BTree::getAddress(const string &path) const {
    return InodeAddress();
}

list<InodeAddress> BTree::getChildren(const string &path) const {
    return list<InodeAddress>();
}

bool BTree::insert(const string &path, InodeAddress addr) {
    return false;
}

bool BTree::remove(const string &path) {
    return false;
}

size_t BTree::getSize() const noexcept {
    return 0;
}


#ifndef BTREE_H
#define BTREE_H

#include "btreekey.h"
#include "btreevalue.h"
#include "btreeleaf.h"
#include "btreefolder.h"
#include "rtfs.h"

class BTree {
public:
    BTree(int _degree);
    void add(BTreeKey key, BTreeValue inode);
    void remove(BTreeKey key);
    BTreeValue get(BTreeKey key);
    void serialize(FileHandler& fileHandler, off_t& addr);
    BTreeFolderEntry* findFilesInFolder(BTreeKey& key);
private:
    int degree;
    BTreeLeaf* root;
    BTreeLeaf* findLeafToAdd(BTreeLeaf* leaf, BTreeEntry* entry);
    BTreeLeaf* findLeafForKey(BTreeLeaf* currentLeaf, BTreeKey& key);
    BTreeLeaf* findLeafForParentKey(BTreeLeaf* currentLeaf, BTreeKey& key);
    void branch(BTreeLeaf* leaf);
    void merge(BTreeLeaf* leaf);
    void serializeLeaf(FileHandler& fileHandler, off_t& addr, BTreeLeaf* leaf);
    void runLeafForFolderEntries(BTreeLeaf* leaf, BTreeKey& key, BTreeFolderEntry*& folderEntry);
};

#endif /* BTREE_H */


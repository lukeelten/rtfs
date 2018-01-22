#ifndef BTREELEAF_H
#define BTREELEAF_H

#include "btreeentry.h"

class BTreeLeaf {
public:
    BTreeLeaf(BTreeLeaf *_parent, int _degree);

    void setParent(BTreeLeaf *parent);

    BTreeLeaf *getParent();

    BTreeLeaf *getBranches();

    BTreeEntry *getEntries();

    int getEntryCount();

    void addEntry(BTreeEntry *entry);

    BTreeEntry *removeEntry(int index);

    BTreeEntry *removeEntry(BTreeKey &key);

    BTreeLeaf *getBranchForKey(BTreeKey &key);

    BTreeLeaf* getBranchForParentKey(BTreeKey& key)

    bool isFull();

    BTreeEntry *getCentralEntry();

    int getBranchCount();

    void addBranch(BTreeLeaf *branch);

    void addBranchAtIndex(int index, BTreeLeaf *branch);

    void setCount(int count);

    bool hasKey(BTreeKey &key);

    int findKey(BTreeKey &key);

    void addEntryAtIndex(int index, BTreeEntry *entry);

    int findParentKey(BTreeKey &key);

    int hasParentKey(BTreeKey &key);

private:
    int degree;
    int entryCount;
    int branchCount;
    BTreeEntry *entries;
    BTreeLeaf *parent;
    BTreeLeaf *branches;
    int findPositionToAdd(BTreeKey &key, int left, int right);
    int findPositionForParentKey(BTreeKey &key, int left, int right);
};

#endif /* BTREELEAF_H */


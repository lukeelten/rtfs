#include "rtfs/btreeleaf.h"
#include <stdlib.h>

BTreeLeaf::BTreeLeaf(BTreeLeaf* _parent, int _degree) : parent(_parent), degree(_degree), entryCount(0), branchCount(0) {
    entries = (BTreeEntry*) malloc(degree * sizeof(BTreeEntry*));
    branches = (BTreeLeaf*) malloc((degree + 1) * sizeof(BTreeLeaf*));
}

void BTreeLeaf::setParent(BTreeLeaf* parent) {
    this->parent = parent;
}

BTreeLeaf* BTreeLeaf::getParent() {
    return this->parent;
}

BTreeLeaf* BTreeLeaf::getBranches() {
    this->branches;
}

BTreeEntry* BTreeLeaf::getEntries() {
    this->entries;
}

int BTreeLeaf::getEntryCount() {
    return this->entryCount;
}

int BTreeLeaf::findPositionToAdd(BTreeKey& key, int left, int right) {
    if (left == right) {
        return left;
    }

    if (key.getHash() < this->entries[(left + right) / 2].getKey().getParenthash()) {
        return findPositionToAdd(key, left, right - 1);
    } else {
        return findPositionToAdd(key, (left + right) / 2, right);
    }
}

int BTreeLeaf::findPositionForParentKey(BTreeKey& key, int left, int right) {
    if (left == right) {
        return left;
    }

    if (key.getParenthash() < this->entries[(left + right) / 2].getKey().getParenthash()) {
        return findPositionForParentKey(key, left, right - 1);
    } else {
        return findPositionForParentKey(key, (left + right) / 2, right);
    }
}

void BTreeLeaf::addEntry(BTreeEntry* entry) {
    addEntryAtIndex(findPositionToAdd(entry->getKey(), 0, this->entryCount), entry);
}

BTreeEntry* BTreeLeaf::removeEntry(int index) {
    BTreeEntry* tmp = this->entries[index];
    for (int i = index; i < this->entryCount - 1; i++) {
        this->entries[i] = this->entries[i + 1];
    }
    this->entryCount--;
}

BTreeEntry* BTreeLeaf::removeEntry(BTreeKey& key) {
    removeEntry(findKey(key));
}

BTreeLeaf* BTreeLeaf::getBranchForKey(BTreeKey& key) {
    if (branchCount == 0) {
        return nullptr;
    }

    int position = findPositionToAdd(key, 0, this->entryCount);
    if (this->entries[position].getKey().getHash() < key.getHash()) {
        return this->branches[position + 1];
    } else {
        return this->branches[position];
    }
}

BTreeLeaf* BTreeLeaf::getBranchForParentKey(BTreeKey& key) {
    if (branchCount == 0) {
        return nullptr;
    }

    int position = findPositionForParentKey(key, 0, this->entryCount);
    if (this->entries[position].getKey().getParenthash() < key.getParenthash()) {
        return this->branches[position + 1];
    } else {
        return this->branches[position];
    }
}

bool BTreeLeaf::isFull() {
    return this->entryCount == this->degree;
}

BTreeEntry* BTreeLeaf::getCentralEntry() {
    return this->entries[this->entryCount / 2];
}

int BTreeLeaf::getBranchCount() {
    return this->branchCount;
}

void BTreeLeaf::addBranch(BTreeLeaf* branch) {
    this->branches[++this->branchCount] = branch;
}

void BTreeLeaf::addBranchAtIndex(int index, BTreeLeaf* branch) {
    for (int i = this->branchCount; i > index; i++) {
        this->branches[i] = this->branches[i - 1];
    }
    this->branches[index] = branch;
}

void BTreeLeaf::setCount(int count) {
    this->entryCount = count;
    this->branchCount = count + 1;
}

int BTreeLeaf::findKey(BTreeKey& key) {
    for (int i = 0; i < this->entryCount; i++) {
        if (this->entries->getKey().getHash() == key.getHash()) {
            return i;
        }
    }
    return -1;
}

int BTreeLeaf::findParentKey(BTreeKey& key) {
    for (int i = 0; i < this->entryCount; i++) {
        if (this->entries->getKey().getParenthash() == key.getParenthash()) {
            return i;
        }
    }
    return -1;
}

bool BTreeLeaf::hasParentKey(BTreeKey& key) {
    return findParentKey(key) != -1;
}

bool BTreeLeaf::hasKey(BTreeKey key) {
    for (int i = 0; i < this->entryCount; i++) {
        if (this->entries->getKey().getHash() == key.getHash()) {
            return true;
        }
    }
    return false;
}

void BTreeLeaf::addEntryAtIndex(int index, BTreeEntry* entry) {
    int position = index;
    if (this->entries[position].getKey().getHash() < entry->getKey().getHash()) {
        position++;
    }
    for (int i = this->entryCount; i > position; i--) {
        this->entries[i] = this->entries[i - 1];
    }
    this->entries[position] = entry;
    this->entryCount++;
}
#include <stddef.h>

#include "rtfs/btree.h"

BTree::BTree(int _degree) : degree(_degree) {
    this->root = new BTreeLeaf(_degree);
}

void BTree::add(BTreeKey key, BTreeValue inode) {
    BTreeEntry* entry = new BTreeEntry(key, inode);
    BTreeLeaf* leaf = findLeafToAdd(this->root, entry);
    leaf->addEntry(entry);
    branch(leaf);
}

void BTree::branch(BTreeLeaf* leaf) {
    if (leaf->isFull()) {
        if (leaf != this->root) {
            BTreeEntry entryToMove = leaf->getCentralEntry();
            BTreeLeaf* rightSplit = new BTreeLeaf(leaf->getParent(), degree);
            for (int i = degree / 2 + 1; i < degree; i++) {
                rightSplit->addEntry(leaf->getEntries()[i]);
            }
            if (leaf->getBranchCount() > 0) {
                for (int i = degree / 2; i < degree; i++) {
                    rightSplit->addBranch(leaf->getBranches()[i]);
                }
            }
            leaf->setCount(degree / 2);
            leaf->getParent()->addEntry(entryToMove);
            branch(leaf->getParent());
        } else {
            BTreeEntry* entryToMove = leaf->getCentralEntry();
            BTreeLeaf* newRoot = new BTreeLeaf(nullptr, degree);
            BTreeLeaf* rightSplit = new BTreeLeaf(newRoot, degree);
            newRoot->addEntry(entryToMove);
            newRoot->addBranch(leaf);
            newRoot->addBranch(rightSplit);
            for (int i = degree / 2 + 1; i < degree; i++) {
                rightSplit->addEntry(leaf->getEntries()[i]);
            }
            if (leaf->getBranchCount() > 0) {
                for (int i = degree / 2; i < degree; i++) {
                    rightSplit->addBranch(leaf->getBranches()[i]);
                }
            }
            leaf->setCount(degree / 2);
        }
    }
}

BTreeLeaf* BTree::findLeafToAdd(BTreeLeaf* leaf, BTreeEntry* entry) {
    BTreeLeaf* tmp = leaf->getBranchForEntry(entry);
    if (leaf->getBranchForEntry(entry) != nullptr) {
        return findLeafToAdd(tmp, entry);
    }
    return leaf;
}

void BTree::remove(BTreeKey& key) {
    BTreeLeaf* leaf = findLeafForKey(root, key);
    int index = leaf->findKey(key);
    leaf->removeEntry(key);
    if (leaf->getBranchCount() != 0) {
        BTreeLeaf* leftBranch = leaf->getBranches()[index];
        BTreeEntry* entryToMove = leftBranch->getEntries()[leftBranch->getEntryCount()];
        leftBranch->removeEntry(leftBranch->getEntryCount());
        leaf->addEntryAtIndex(index, entryToMove);
    } else {

    }
}

BTreeLeaf* BTree::findLeafForKey(BTreeLeaf* currentLeaf, BTreeKey& key) {
    if (currentLeaf->hasKey(key)) {
        return currentLeaf;
    } else {
        return findLeafForKey(currentLeaf->getBranchForKey(key));
    }
}

BTreeValue BTree::get(BTreeKey key) {
    BTreeLeaf* leaf = findLeafForKey(root, key);
    return leaf->getEntries()[leaf->findKey(key)].getValue();
}

BTreeLeaf* BTree::findLeafForParentKey(BTreeLeaf* currentLeaf, BTreeKey& key) {
    if (currentLeaf->hasParentKey(key)) {
        /*if (currentLeaf->getBranchCount() > 0) {
            int index = currentLeaf->findParentKey(key);
            if (currentLeaf->getBranches()[index]->hasParentKey(key)) {
                findLeafForKey(currentLeaf->getBranches()[index], key);
            } else {
                return currentLeaf;
            }
        } else {
            return currentLeaf;
        }*/
        return currentLeaf;
    } else {
        return findLeafForParentKey(currentLeaf->getBranchForParentKey(key), key);
    }
}

BTreeFolderEntry* BTree::findFilesInFolder(BTreeKey& key) {
    BTreeFolderEntry* dummy = new BTreeFolderEntry(nullptr, nullptr);
    BTreeLeaf* start = findLeafForParentKey(root, key);
    
}

void BTree::runLeafForFolderEntries(BTreeLeaf* leaf, BTreeKey& key, BTreeFolderEntry*& folderEntry) {
    for (int i = leaf->findParentKey(key); i < leaf->getEntryCount(); i++) {
        if (leaf->getEntries()[i]->getKey().getParenthash() != key.getParenthash()) {
            break;
        }
        folderEntry->next = new BTreeFolderEntry(nullptr, leaf->getEntries()[i].getValue());
        folderEntry = folderEntry->next;
        if (leaf->getBranches()[i + 1]->hasParentKey(key)) {
            runLeafForFolderEntries(leaf->getBranches()[i + 1], key, folderEntry);
        }
    }
}

void BTree::serialize(FileHandler& fileHandler, off_t& addr) {
    serializeLeaf(fileHandler, addr, root);
}

void BTree::serializeLeaf(FileHandler& fileHandler, off_t& addr, BTreeLeaf* leaf) {
    for (int i = 0; i < leaf->getEntryCount(); i++) {
        BTreeEntry* entry = leaf->getEntries()[i];
        fileHandler.write<unsigned long>(entry->getKey().getHash(), addr);
        addr += sizeof (entry->getKey().getHash());
        fileHandler.write<unsigned long>(entry->getValue().inode, addr);
        addr += sizeof (entry->getValue().inode);
    }
    for (int i = 0; i < leaf->getBranchCount(); i++) {
        BTreeLeaf* branch = leaf->getBranches()[i];
        serializeLeaf(fileHandler, addr, branch);
    }
}
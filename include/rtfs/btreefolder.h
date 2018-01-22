#ifndef BTREEFOLDER_H
#define BTREEFOLDER_H

#include "btreevalue.h"

class BTreeFolderEntry {
public:
    BTreeFolderEntry* next;
    BTreeValue inode;
    
    BTreeFolderEntry(BTreeFolderEntry* _next, BTreeValue _inode): next(_next), inode(_inode) {};
};

#endif /* BTREEFOLDER_H */


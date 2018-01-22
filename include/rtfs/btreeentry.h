#ifndef BTREEENTRY_H
#define BTREEENTRY_H

#include "btreekey.h"
#include "btreevalue.h"

class BTreeEntry {
public:
    BTreeEntry(BTreeKey key, BTreeValue value);
    BTreeKey getKey();
    BTreeValue getValue();
private:
    BTreeKey key;
    BTreeValue value;
};

#endif /* BTREEENTRY_H */


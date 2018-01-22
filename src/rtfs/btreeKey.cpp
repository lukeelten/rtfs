#include "rtfs/btreekey.h"

explicit BTreeKey::BTreeKey(unsigned long hash_) : hash(hash_) {}

unsigned long BTreeKey::getFilehash() const noexcept {
    return hash & 0x00000000FFFFFFFF;
}

unsigned long BTreeKey::getParenthash() const noexcept {
    return hash & 0xFFFFFFFF00000000;
}

unsigned long BTreeKey::getHash() const noexcept {
    return hash;
}
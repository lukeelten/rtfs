#ifndef BTREEKEY_H
#define BTREEKEY_H

class BTreeKey {
public:
    explicit BTreeKey(unsigned long hash_);
    unsigned long getFilehash() const noexcept;
    unsigned long getParenthash() const noexcept;
    unsigned long getHash() const noexcept;

private:
    unsigned long hash;
};

#endif /* BTREEKEY_H */


#ifndef RTFS_RTFS_H
#define RTFS_RTFS_H


class Key {

public:
    explicit Key(unsigned long hash_) : hash(hash_) {}

    unsigned long getFilehash() const noexcept  {
        return hash & 0x00000000FFFFFFFF;
    }

    unsigned long getParenthash() const noexcept  {
        return hash & 0xFFFFFFFF00000000;
    }

    unsigned long getHash() const noexcept  {
        return hash;
    }

private:
    unsigned long hash;
};


#endif //RTFS_RTFS_H

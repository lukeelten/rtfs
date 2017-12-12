
#ifndef RTFS_INODE_ADDRESS_H
#define RTFS_INODE_ADDRESS_H


#include <sys/stat.h>
#include <string>

class Inode;


class InodeAddress {
public:
    InodeAddress() noexcept : addr_(0) {};
    explicit InodeAddress(off_t addr) noexcept : addr_(addr) {}

    ~InodeAddress() = default;
    InodeAddress(const InodeAddress& ) = default;
    InodeAddress(InodeAddress&& ) = default;
    InodeAddress& operator = (const InodeAddress& ) = default;
    InodeAddress& operator = (InodeAddress&& ) = default;

    Inode readInode() const;

    off_t getAddress() const noexcept { return addr_; }
    explicit operator off_t() const noexcept { return addr_;}

private:
    off_t addr_;
};


class InodeAddressHasher {
public:
    size_t operator () (InodeAddress addr) const noexcept;
};


// Non  Member Functions
bool operator == (const InodeAddress& lhs, const InodeAddress& rhs) noexcept;
bool operator == (const InodeAddress& lhs, off_t rhs) noexcept;
bool operator == (off_t lhs, const InodeAddress& rhs) noexcept;
bool operator != (const InodeAddress& lhs, const InodeAddress& rhs) noexcept;
bool operator != (const InodeAddress& lhs, off_t rhs) noexcept;
bool operator != (off_t lhs, const InodeAddress& rhs) noexcept;

bool operator > (const InodeAddress& lhs, const InodeAddress& rhs) noexcept;
bool operator > (off_t lhs, const InodeAddress& rhs) noexcept;
bool operator > (const InodeAddress& lhs, off_t rhs) noexcept;
bool operator >= (const InodeAddress& lhs, const InodeAddress& rhs) noexcept;
bool operator >= (off_t lhs, const InodeAddress& rhs) noexcept;
bool operator >= (const InodeAddress& lhs, off_t rhs) noexcept

bool operator < (const InodeAddress& lhs, const InodeAddress& rhs) noexcept;
bool operator < (off_t lhs, const InodeAddress& rhs) noexcept;
bool operator < (const InodeAddress& lhs, off_t rhs) noexcept;
bool operator <= (const InodeAddress& lhs, const InodeAddress& rhs) noexcept;
bool operator <= (off_t lhs, const InodeAddress& rhs) noexcept;
bool operator <= (const InodeAddress& lhs, off_t rhs) noexcept;

InodeAddress operator += (const InodeAddress& lhs, const InodeAddress& rhs) noexcept;
InodeAddress operator += (off_t lhs, const InodeAddress& rhs) noexcept;
InodeAddress operator += (const InodeAddress&lhs, off_t rhs) noexcept;
InodeAddress operator -= (const InodeAddress& lhs, const InodeAddress& rhs) noexcept;
InodeAddress operator -= (off_t lhs, const InodeAddress& rhs) noexcept;
InodeAddress operator -= (const InodeAddress&lhs, off_t rhs) noexcept;

#endif //RTFS_INODE_ADDRESS_H

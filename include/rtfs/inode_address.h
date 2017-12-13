
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
    operator off_t() const noexcept { return addr_;}

    operator bool() const noexcept { return addr_ > 0; }

private:
    off_t addr_;
};

// Hash function to use InodeAddress in unordered_map
namespace std {
    template<>
    class hash<InodeAddress> {
    public:
        size_t operator() (const InodeAddress& addr) const {
            return hasher(addr.getAddress());
        }

    private:
        static hash<off_t> hasher; // will be instantiated thread safe
    };
}

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


#include <sys/types.h>

#include "rtfs/inode.h"



Inode Inode::initEmpty(InodeAddress addr) {
    Inode inode;
    inode.addr_ = addr;

    inode.type_ = TYPE_UNKNOWN;

    inode.parent_ = InodeAddress(0);
    inode.next_ = InodeAddress(0);

    inode.length_ = 0;

    inode.atime_ = 0;
    inode.ctime_ = 0;

    inode.mode_ = 0;

    inode.gid_ = 0;
    inode.uid_ = 0;

    return inode;
}

bool operator == (const InodeAddress& lhs, const InodeAddress& rhs) noexcept {
    return lhs.getAddress() == rhs.getAddress();
}

bool operator == (const InodeAddress& lhs, off_t rhs) noexcept {
    return lhs.getAddress() == rhs;
}

bool operator == (off_t lhs, const InodeAddress& rhs) noexcept {
    return lhs == rhs.getAddress();
}

bool operator != (const InodeAddress& lhs, const InodeAddress& rhs) noexcept {
    return lhs.getAddress() != rhs.getAddress();
}

bool operator != (const InodeAddress& lhs, off_t rhs) noexcept {
    return lhs.getAddress() != rhs;
}

bool operator != (off_t lhs, const InodeAddress& rhs) noexcept {
    return lhs != rhs.getAddress();
}

size_t InodeAddressHasher::operator() (InodeAddress addr) const noexcept {
    std::hash<off_t> hash;
    return hash(addr.getAddress());
}

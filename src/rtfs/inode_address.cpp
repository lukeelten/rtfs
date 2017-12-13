

#include <stdexcept>

#include "rtfs/inode.h"
#include "rtfs/instance.h"
#include "rtfs/inode_address.h"

size_t InodeAddressHasher::operator() (InodeAddress addr) const noexcept {
    std::hash<off_t> hash;
    return hash(addr.getAddress());
}

Inode InodeAddress::readInode() const {
    RtfsInstance* instance = RtfsInstance::getInstance();

    Inode inode;
    if (instance->getFile().read(&inode, getAddress())) {
        return std::move(inode);
    }

    throw std::runtime_error("Error while read inode");
}

// Important operators
// Non-members of class for obvious reasons
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

bool operator>(const InodeAddress &lhs, const InodeAddress &rhs) noexcept {
    return lhs.getAddress() > rhs.getAddress();
}

bool operator>(off_t lhs, const InodeAddress &rhs) noexcept {
    return lhs > rhs.getAddress();
}

bool operator>(const InodeAddress &lhs, off_t rhs) noexcept {
    return lhs.getAddress() > rhs;
}

bool operator>=(const InodeAddress &lhs, const InodeAddress &rhs) noexcept {
    return lhs.getAddress() >= rhs.getAddress();
}

bool operator>=(off_t lhs, const InodeAddress &rhs) noexcept {
    return lhs >= rhs.getAddress();
}

bool operator>=(const InodeAddress &lhs, off_t rhs) noexcept {
    return lhs.getAddress() >= rhs;
}

bool operator<(const InodeAddress &lhs, const InodeAddress &rhs) noexcept {
    return lhs.getAddress() < rhs.getAddress();
}

bool operator<(off_t lhs, const InodeAddress &rhs) noexcept {
    return lhs < rhs.getAddress();
}

bool operator<(const InodeAddress &lhs, off_t rhs) noexcept {
    return lhs.getAddress() < rhs;
}

bool operator<=(const InodeAddress &lhs, const InodeAddress &rhs) noexcept {
    return lhs.getAddress() <= rhs.getAddress();
}

bool operator<=(off_t lhs, const InodeAddress &rhs) noexcept {
    return lhs <= rhs.getAddress();
}

bool operator<=(const InodeAddress &lhs, off_t rhs) noexcept {
    return lhs.getAddress() <= rhs;
}


#include "rtfs/instance.h"
#include "rtfs/inode_address.h"
#include "rtfs/inode.h"


Inode Inode::initEmpty(InodeAddress addr) {
    Inode inode;
    inode.clear();

    return inode;
}

bool Inode::save() const noexcept {
    RtfsInstance* instance = RtfsInstance::getInstance();
    return instance->getFile().write(this, addr_.getAddress());
}

void Inode::clear() noexcept {
    type_ = TYPE_EMPTY;

    parent_ = InodeAddress(0);
    next_ = InodeAddress(0);

    length_ = 0;

    atime_ = 0;
    ctime_ = 0;

    mode_ = 0;

    gid_ = 0;
    uid_ = 0;
}


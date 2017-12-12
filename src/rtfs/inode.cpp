

#include "rtfs/instance.h"
#include "rtfs/inode_address.h"
#include "rtfs/inode.h"


Inode Inode::initEmpty(InodeAddress addr) {
    Inode inode;
    inode.addr_ = addr;

    inode.type_ = TYPE_EMPTY;

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

bool Inode::save() const noexcept {
    RtfsInstance* instance = RtfsInstance::getInstance();

    fseek(instance->getFile(), addr_.getAddress(), SEEK_SET);
    return fwrite(this, sizeof(Inode), 1, instance->getFile()) == sizeof(Inode);
}


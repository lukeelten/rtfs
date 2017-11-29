


#include "rtfs/inode.h"



Inode Inode::initEmpty(InodeAddress addr) {
    Inode inode;
    inode.addr_ = addr;

    inode.type_ = TYPE_UNKNOWN;

    inode.parent_ = 0;
    inode.next_ = 0;

    inode.length_ = 0;

    inode.atime_ = 0;
    inode.ctime_ = 0;

    inode.mode_ = 0;

    inode.gid_ = 0;
    inode.uid_ = 0;

    return inode;
}
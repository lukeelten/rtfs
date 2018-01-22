
#include <stdexcept>
#include <memory>

#include "rtfs/folder.h"
#include "rtfs/file.h"
#include "rtfs/instance.h"


#include "rtfs/block.h"


using namespace std;

RtfsBlock* RtfsBlock::readFromDisk(const InodeAddress &addr) {
    RtfsInstance* instance = RtfsInstance::getInstance();

    if (!addr) {
        throw runtime_error("Invalid inode address");
    }
    Inode inode;
    instance->getFile().read(&inode, addr.getAddress());

    switch (inode.getType()) {
        case TYPE_DIR:
            return new RtfsFolder(inode);

        case TYPE_FILE:
            return new RtfsFile(inode);

        case TYPE_EMPTY:
        case TYPE_ALLOCATED:
            return new RtfsBlock(inode);

        default:
            throw runtime_error("Invalid type");
    }
}

bool RtfsBlock::rename(const string& name) {
    inode.setFilename(name);
    return inode.save();
}

bool RtfsBlock::unlink() {
    Inode copy = inode; // Backup
    inode.clear();

    if (inode.save()) {
        return true;
    }

    inode = move(copy);
    return false;
}

bool RtfsBlock::updateOwner(uid_t uid, gid_t gid) {
    inode.setUid(uid);
    inode.setGid(gid);
    return inode.save();
}

bool RtfsBlock::updateMode(mode_t mode) {
    inode.setMode(mode);
    return inode.save();
}

off_t RtfsBlock::getSize() const noexcept {
    return 0;
}


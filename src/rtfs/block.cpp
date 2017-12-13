
#include <memory>
#include <stdexcept>
#include <string>
#include <cstdio>

#include "rtfs/instance.h"

#include "rtfs/block.h"


using namespace std;

static std::shared_ptr<RtfsBlock> RtfsBlock::readFromDisk(const InodeAddress &addr) {
    RtfsInstance* instance = RtfsInstance::getInstance();

    if (!addr) {
        throw runtime_error("Invalid inode address");
    }
    Inode inode;
    instance->getFile().read(&inode, addr.getAddress());

    switch (inode.getType()) {
        case TYPE_DIR:
            auto ret = make_shared<RtfsFolder>(move(inode));
            return ret;

        case TYPE_FILE:
            auto ret = make_shared<RtfsFile>(move(inode));
            return ret;

        case TYPE_EMPTY:
        case TYPE_ALLOCATED:
            auto ret = make_shared<RtfsBlock>(move(inode));
            return ret;

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
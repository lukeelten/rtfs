
#include <stdexcept>
#include <cmath>

#include "rtfs/inode.h"
#include "rtfs/inode_address.h"
#include "rtfs/instance.h"
#include "rtfs/file.h"


using namespace std;

RtfsFile::RtfsFile(off_t size_) : inodes(), size(size_) {
    /*
     * The performance of this implementation is very bad
     * It just searches all inodes of the disk to find free ones.
     */

    RtfsInstance* instance = RtfsInstance::getInstance();

    auto blockSize = static_cast<double>(instance->getSuperblock().getBlockSize() - sizeof(Inode));
    auto result = static_cast<double>(size) / blockSize;
    auto blocks = ceil(result); // Total numbers of blocks

    off_t base = instance->getSuperblock().getRoot().getAddress();
    instance->getFile().setPosition(base);

    // Start with 1 because we skip root
    for (int i = 1; i < instance->getSuperblock().getNumInodes(); i++) {
        Inode inode;
        instance->getFile().read(&inode, instance->getSuperblock().getBlockSize(), SEEK_CUR;

        if (inode.getType() == TYPE_EMPTY) {
            inode.setType(TYPE_ALLOCATED);

            if (inode.save()) {
                inodes.push_back(inode.getAddress());
            }
        }

        if (inodes.size() >= blocks) {
            break;
        }
    }

    if (inodes.size() < blocks) {
        throw bad_alloc();
    }
}


RtfsFile::RtfsFile(const InodeAddress& addr) : inodes(), size(0) {
    RtfsInstance* instance = RtfsInstance::getInstance();

    if (!instance->getFile().read(&inode, addr)) {
        throw runtime_error("Error while reading file root inode");
    }

    if (inode.getType() != TYPE_FILE) {
        throw runtime_error("Invalid inode type");
    }

    inodes.push_back(addr);
    loadFile();
}

RtfsFile::RtfsFile(const Inode &inode_) : RtfsBlock(inode_), inodes(), size(0) {
    if (inode.getType() != TYPE_FILE) {
        throw runtime_error("Invalid inode type");
    }

    inodes.push_back(inode.getAddress());
    loadFile();
}

void RtfsFile::loadFile() {
    RtfsInstance* instance = RtfsInstance::getInstance();

    if (inode.getAddress() < instance->getSuperblock().getRoot()) {
        // Root address cannot be below of root folder
        throw runtime_error("Invalid root address given");
    }

    off_t totalSize = inode.getLength();
    Inode current = inode; // Makes copy; intended!
    while (current.hasNext()) {
        current = current.getNext();
        totalSize += current.getLength();
        inodes.push_back(current.getAddress());
    }

}

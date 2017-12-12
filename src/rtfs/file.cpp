
#include <stdexcept>
#include <cstdio>
#include <cmath>

#include "rtfs/inode.h"
#include "rtfs/inode_address.h"
#include "rtfs/instance.h"
#include "rtfs/file.h"


using namespace std;

RtfsFile::RtfsFile(off_t size_) : root(), inodes(), size(size_) {
    /*
     * The performance of this implementation is very bad
     * It just searches all inodes of the disk to find free ones.
     */

    RtfsInstance* instance = RtfsInstance::getInstance();

    auto blockSize = static_cast<double>(instance->getSuperblock().getBlockSize() - sizeof(Inode));
    auto result = static_cast<double>(size) / blockSize;
    auto blocks = ceil(result); // Total numbers of blocks

    off_t base = instance->getSuperblock().getRoot().getAddress();
    fseek(instance->getFile(), base, SEEK_SET);

    // Start with 1 because we skip root
    for (int i = 1; i < instance->getSuperblock().getNumInodes(); i++) {
        fseek(instance->getFile(), instance->getSuperblock().getBlockSize(), SEEK_CUR);
        Inode inode;
        fread(&inode, sizeof(Inode), 1, instance->getFile());

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


RtfsFile::RtfsFile(const InodeAddress& addr) : root(), inodes(), size(0) {
    RtfsInstance* instance = RtfsInstance::getInstance();

    fseek(instance->getFile(), addr.getAddress(), SEEK_SET);
    if (fread(&root, sizeof(Inode), 1, instance->getFile()) != sizeof(Inode)) {
        throw runtime_error("Error while reading file root inode");
    }

    if (root.getType() != TYPE_FILE) {
        throw runtime_error("Invalid inode type");
    }

    inodes.push_back(addr);
    loadFile();
}

RtfsFile::RtfsFile(const Inode &inode_) : root(inode_), inodes(), size(0) {
    if (root.getType() != TYPE_FILE) {
        throw runtime_error("Invalid inode type");
    }

    inodes.push_back(root.getAddress());
    loadFile();
}

void RtfsFile::loadFile() {
    RtfsInstance* instance = RtfsInstance::getInstance();

    if (root.getAddress() < instance->getSuperblock().getRoot()) {
        // Root address cannot be below of root folder
        throw runtime_error("Invalid root address given");
    }

    off_t totalSize = root.getLength();
    Inode current = root; // Makes copy; intended!
    while (current.hasNext()) {
        current = current.getNext();
        totalSize += current.getLength();
        inodes.push_back(current.getAddress());
    }

}

bool RtfsFile::rename(const string &name) {
    return false;
}


#include <string>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <sys/stat.h>

#include "rtfs.h"

#include "log.h"

using namespace std;


RtfsInstance::RtfsInstance(const string& file_) : file(file_), superblock(), root(), config(nullptr), openFiles(), openAddresses(), openFolders(), counter(0) {
}

RtfsInstance* RtfsInstance::init(struct fuse_config* config_) noexcept {
    if (!file.exists()) {
        throw runtime_error("File not exists");
    }

    config = config_;
    file.open();

    superblock = Superblock::readFromDisk();
    root = superblock.getRoot().readInode();
    openFiles.clear(); // In theory this should be empty anyway

    if (superblock.getVersion() != RTFS_VERSION) {
        throw runtime_error("Invalid RTFS version found.");
    }

    return this;
}

bool RtfsInstance::openFile(string filename, struct fuse_file_info* fi) {

    // @todo find in tree
    InodeAddress addr;

    try {
        if (openAddresses.find(addr) != openAddresses.end()) {
            return false; // FIle already open
        }

        FileDescriptor fd = getNextDescriptor();
        openFiles[fd] = make_shared<RtfsFile>(addr);
        openAddresses[addr] = fd;
        fi->fh = fd;

        return true;
    } catch(std::exception& ex) {
        Log::getInstance() << ex;
        return false;
    }
}

FileDescriptor RtfsInstance::getNextDescriptor() {
    return counter++; // Atomic operation
}

shared_ptr<RtfsFile> RtfsInstance::getOpenFile(FileDescriptor fd) {
    if (openFiles.find(fd) != openFiles.end()) {
        return openFiles[fd];
    }
    return shared_ptr<RtfsFile>();
}


shared_ptr<RtfsFolder> RtfsInstance::getOpenFolder(FileDescriptor fd) {
    if (openFolders.find(fd) != openFolders.end()) {
        return openFolders[fd];
    }
    return shared_ptr<RtfsFolder>();
}

shared_ptr<RtfsBlock> RtfsInstance::getOpen(FileDescriptor fd) {
    auto file = openFiles.find(fd);
    if (file == openFiles.end()) {
        auto folder = openFolders.find(fd);

        if (folder != openFolders.end()) {
            return folder->second;
        }

        return shared_ptr<RtfsBlock>();
    }

    return file->second;
}

shared_ptr<RtfsBlock> RtfsInstance::getOpen(InodeAddress addr) {
    auto fd = openAddresses.find(addr);
    if (fd != openAddresses.end()) {
        return getOpen(fd->second);
    }

    return shared_ptr<RtfsBlock>();
}

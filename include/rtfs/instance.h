
#ifndef RTFS_INSTANCE_H
#define RTFS_INSTANCE_H

#include <fuse3/fuse.h>
#include <cstdio>
#include <string>
#include <atomic>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <sys/stat.h>

#include "superblock.h"
#include "file.h"
#include "folder.h"
#include "file_handler.h"
#include "../log.h"
#include "btree.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;

typedef uint64_t FileDescriptor;

class RtfsInstance {

public:
    RtfsInstance(const string& file_);
    ~RtfsInstance() = default; // Default is fine for us

    // No copy & No move
    RtfsInstance(const RtfsInstance& ) = delete;
    RtfsInstance& operator = (const RtfsInstance& ) = delete;
    RtfsInstance(RtfsInstance&& ) = delete;
    RtfsInstance& operator = (RtfsInstance&& ) = delete;

    // Init FS; Called by fuse
    RtfsInstance* init (struct fuse_config* config_);

    bool openFile(string filename, struct fuse_file_info* fi);
    bool openFolder(string name, struct fuse_file_info* fi);

    const Superblock& getSuperblock() const noexcept { return superblock; }
    const Inode& getInode() const noexcept { return root; }

    static RtfsInstance* getInstance() noexcept {
        return static_cast<RtfsInstance*>(fuse_get_context()->private_data);
    }

    const FileHandler& getFile() const noexcept {
        // In practice not really const, but what ever ....
        return file;
    }

    shared_ptr<RtfsBlock> getOpenFile(FileDescriptor fd);
    shared_ptr<RtfsBlock> getOpenFolder(FileDescriptor fd);
    shared_ptr<RtfsBlock> getOpen(FileDescriptor fd);
    shared_ptr<RtfsBlock> getOpen(InodeAddress addr);

    bool close(FileDescriptor fd);
    bool close(shared_ptr<RtfsBlock> block);

    bool isOpen(InodeAddress addr) const { return openAddresses.find(addr) != openAddresses.end();}

    shared_ptr<BTree> getTree() const noexcept { return tree; }

private:
    FileDescriptor getNextDescriptor();

    FileHandler file;

    Superblock superblock;
    Inode root;
    shared_ptr<BTree> tree;

    // Will be handled by fuse, DO NOT DELETE
    fuse_config* config;

    unordered_map<FileDescriptor, shared_ptr<RtfsBlock>> openFiles;
    unordered_map<InodeAddress, FileDescriptor> openAddresses;
    unordered_map<FileDescriptor, shared_ptr<RtfsBlock>> openFolders;

    std::atomic<FileDescriptor> counter;
};

#endif //RTFS_INSTANCE_H

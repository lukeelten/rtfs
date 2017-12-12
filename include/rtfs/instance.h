
#ifndef RTFS_INSTANCE_H
#define RTFS_INSTANCE_H

#include <fuse.h>
#include <cstdio>
#include <string>
#include <mutex>
#include <unordered_set>
#include <functional>

#include "superblock.h"

using std::string;
using std::mutex;
using std::function;
using std::unordered_set;

class RtfsInstance {

public:
    RtfsInstance(string file_);
    ~RtfsInstance(); // Default is fine for us

    // No copy & No move
    RtfsInstance(const RtfsInstance& ) = delete;
    RtfsInstance& operator = (const RtfsInstance& ) = delete;
    RtfsInstance(RtfsInstance&& ) = delete;
    RtfsInstance& operator = (RtfsInstance&& ) = delete;

    // Init FS; Called by fuse
    RtfsInstance* init (struct fuse_config* config_) noexcept;

    bool openFile(string filename, struct fuse_file_info* fi);

    const Superblock& getSuperblock() const noexcept { return superblock; }
    const Inode& getInode() const noexcept { return root; }
    const string& getFilename() const noexcept { return filename; }

    static RtfsInstance* getInstance() noexcept {
        return static_cast<RtfsInstance*>(fuse_get_context()->private_data);
    }

    FILE* getFile() const noexcept {
        // In practice not really const, but what ever ....
        return file;
    }

private:
    const string filename;
    FILE* file;

    Superblock superblock;
    Inode root;

    // Will be handled by fuse, DO NOT DELETE
    fuse_config* config;

    mutex lock;
    unordered_set<InodeAddress, InodeAddressHasher> openFiles;
};




#endif //RTFS_INSTANCE_H

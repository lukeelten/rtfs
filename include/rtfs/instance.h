
#ifndef RTFS_INSTANCE_H
#define RTFS_INSTANCE_H

#include <cstdio>
#include <memory>
#include <string>

#include "superblock.h"

using std::string;
using std::unique_ptr;

class RtfsInstance {

public:
    RtfsInstance(const string& file);
    ~RtfsInstance() = default; // Default is fine for us

    // No copy
    RtfsInstance(const RtfsInstance& ) = delete;
    RtfsInstance& operator = (const RtfsInstance& ) = delete;

    // Move is ok
    RtfsInstance(RtfsInstance&& ) = default;
    RtfsInstance& operator = (RtfsInstance&& ) = default;

    const Superblock& getSuperblock() const noexcept { return superblock; }
    const Inode& getInode() const noexcept { return root; }
    string getFilename() const noexcept { return filename; }

private:
    const string filename;
    unique_ptr<FILE> file;

    const Superblock superblock;
    Inode root;
};




#endif //RTFS_INSTANCE_H

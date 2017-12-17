
#ifndef RTFS_TYPES_H
#define RTFS_TYPES_H

#include <memory>
#include <string>

#include "inode.h"


class RtfsBlock {
protected:
    RtfsBlock();
    RtfsBlock(Inode inode_);

    virtual void readType() = 0;

public:
    // Constructor has to be virtual
    virtual ~RtfsBlock() = default;

    // Copy & Move is fine
    RtfsBlock(const RtfsBlock& ) = default;
    RtfsBlock(RtfsBlock&& ) = default;
    RtfsBlock& operator = (const RtfsBlock& ) = default;
    RtfsBlock&operator = (RtfsBlock&& ) = default;

    // Reads Inode from disks and creates appropriate type
    static std::shared_ptr<RtfsBlock> readFromDisk(const InodeAddress& addr);

    const Inode& getInode() const noexcept { return inode; }

    bool rename(const std::string& name);
    bool unlink();

    bool updateOwner(uid_t uid, gid_t gid);
    bool updateMode(mode_t mode);



protected:
    Inode inode;

};



#endif //RTFS_TYPES_H

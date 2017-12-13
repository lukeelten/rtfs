
#ifndef RTFS_TYPES_H
#define RTFS_TYPES_H

#include <memory>
#include <string>

#include "inode.h"


class RtfsBlock {
protected:
    RtfsBlock(Inode inode_);

    virtual void readType() = 0;

public:
    virtual ~RtfsBlock() = default;
    RtfsBlock(const RtfsBlock& ) = default;


    static std::shared_ptr<RtfsBlock> readFromDisk(const InodeAddress& addr);

    const Inode& getInode() const noexcept { return inode; }

    bool rename(const std::string& name);
    bool unlink();

    bool write(const char* data, size_t bytes);
    bool read(char* buffer, size_t bytes);

private:
    Inode inode;

};



#endif //RTFS_TYPES_H

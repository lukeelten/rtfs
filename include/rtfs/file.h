
#ifndef RTFS_FILE_H
#define RTFS_FILE_H

#include <list>

#include "block.h"
#include "inode.h"

using std::list;

class RtfsFile : public RtfsBlock {
public:
    RtfsFile() noexcept {}
    RtfsFile(off_t size); // Allocate new file with this size
    explicit RtfsFile(const InodeAddress& addr); // Reads file from disk
    explicit RtfsFile(const Inode& inode); // Init from existing inode;


    ~RtfsFile() = default;
    RtfsFile(const RtfsFile& ) = default;
    RtfsFile(RtfsFile&& ) = default;
    RtfsFile& operator = (const RtfsFile& ) = default;
    RtfsFile& operator = (RtfsFile&& ) = default;

    off_t getSize() const noexcept { return size; }

    bool resize(off_t length);

private:
    void loadFile();

    list<InodeAddress> inodes;
    off_t size; // Total file size
};


#endif //RTFS_FILE_H

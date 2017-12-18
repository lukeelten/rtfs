
#ifndef RTFS_FILE_H
#define RTFS_FILE_H

#include <list>

#include "block.h"
#include "inode.h"

using std::list;

class RtfsFile : public RtfsBlock {
public:
    RtfsFile() noexcept : inodes(), size(0) {}
    RtfsFile(off_t size); // Allocate new file with this size
    explicit RtfsFile(const InodeAddress& addr); // Reads file from disk
    explicit RtfsFile(const Inode& inode); // Init from existing inode;


    ~RtfsFile() = default;
    RtfsFile(const RtfsFile& ) = default;
    RtfsFile(RtfsFile&& ) = default;
    RtfsFile& operator = (const RtfsFile& ) = default;
    RtfsFile& operator = (RtfsFile&& ) = default;

    bool resize(off_t length);

    off_t getSize() const noexcept override;

private:
    void loadFile();

    list<InodeAddress> inodes;
    off_t size; // Total file size
};


#endif //RTFS_FILE_H

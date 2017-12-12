
#ifndef RTFS_FILE_H
#define RTFS_FILE_H

#include <list>
#include <string>

#include "inode.h"

using std::list;
using std::string;

class RtfsFile {
public:
    RtfsFile(off_t size); // Allocate new file with this size
    explicit RtfsFile(const InodeAddress& addr); // Reads file from disk
    explicit RtfsFile(const Inode& inode); // Init from existing inode;


    ~RtfsFile() = default;
    RtfsFile(const RtfsFile& ) = default;
    RtfsFile(RtfsFile&& ) = default;
    RtfsFile& operator = (const RtfsFile& ) = default;
    RtfsFile& operator = (RtfsFile&& ) = default;

    bool rename(const string& name);

private:
    void loadFile();

    Inode root;
    list<InodeAddress> inodes;
    off_t size; // Total file size
};


#endif //RTFS_FILE_H

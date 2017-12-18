
#ifndef RTFS_FOLDER_H
#define RTFS_FOLDER_H

#include <vector>
#include <functional>
#include <string>

#include "block.h"
#include "inode_address.h"

using std::vector;
using std::function;
using std::string;

class RtfsFolder : public RtfsBlock {
public:
    RtfsFolder() noexcept {}
    ~RtfsFolder() = default;

    RtfsFolder(const RtfsFolder& ) = default;
    RtfsFolder(RtfsFolder&& ) = default;
    RtfsFolder& operator = (const RtfsFolder& ) = default;
    RtfsFolder& operator = (RtfsFolder&& ) = default;



    size_t numChildren() const noexcept { return children.size(); }

    bool readFolder(off_t start, function<bool(string, off_t)> filler);

private:
    vector<InodeAddress> children;

};


#endif //RTFS_FOLDER_H

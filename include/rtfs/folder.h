
#ifndef RTFS_FOLDER_H
#define RTFS_FOLDER_H

#include <vector>

#include "block.h"
#include "inode_address.h"

using std::vector;

class RtfsFolder : public RtfsBlock {
public:


private:
    vector<InodeAddress> children;

};


#endif //RTFS_FOLDER_H

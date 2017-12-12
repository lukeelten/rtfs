
#ifndef RTFS_FOLDER_H
#define RTFS_FOLDER_H

#include <vector>

#include "inode.h"
#include "inode_address.h"

using std::vector;

class RtfsFolder {
public:


private:
    Inode inode;
    vector<InodeAddress> children;

};


#endif //RTFS_FOLDER_H

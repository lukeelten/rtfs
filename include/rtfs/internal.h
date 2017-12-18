
#ifndef RTFS_INTERNAL_H
#define RTFS_INTERNAL_H

#include <fuse.h>

#ifndef INTERNAL
#define INTERNAL static
#endif

// Error Codes
#define ERR_SUCCESS (0)
#define ERR_NOT_IMPLEMENTED (-999)
#define ERR_GENERAL_ERROR (-1)
#define ERR_ACTION_FAILED (-2)
#define ERR_INVALID_HANLE (-3)

// Class to encapsulate operations struct
class RtfsOperations {
public:
    RtfsOperations() = delete;


    static struct fuse_operations getOperations() noexcept {
        return operations_;
    }

private:
    static struct fuse_operations operations_;
};




#endif //RTFS_INTERNAL_H

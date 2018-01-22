
#ifndef RTFS_INTERNAL_H
#define RTFS_INTERNAL_H

#include <fuse3/fuse.h>

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
    RtfsOperations();

    struct fuse_operations getOperations() const noexcept {
        return operations_;
    }

private:
    struct fuse_operations operations_;
};




#endif //RTFS_INTERNAL_H

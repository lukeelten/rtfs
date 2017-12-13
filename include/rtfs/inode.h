
#ifndef RTFS_INODE_H
#define RTFS_INODE_H

#include <sys/stat.h>
#include <string>
#include <cstring>

#include "inode_address.h"

// Typedefs
typedef unsigned char u8;

// Definition of different innode types
#define TYPE_UNKNOWN 0
#define TYPE_EMPTY 1
#define TYPE_ALLOCATED 2
#define TYPE_FILE 3
#define TYPE_DIR 4
#define TYPE_SYMLINK 5


using std::string;

class Inode {
public:
    static Inode initEmpty(InodeAddress addr);

    const InodeAddress& getAddress() const noexcept { return addr_; }
    void setAddress(InodeAddress addr) noexcept { addr_ = addr; }

    u8 getType() const noexcept { return type_; }
    void setType(u8 type) noexcept { type_ = type; }

    string getFilename() const noexcept { return string(filename_); }
    void setFilename(const string& filename) noexcept { strncpy(filename_, filename.c_str(), 128); filename_[filename.size()] = '\0'; }

    bool hasParent() const noexcept { return parent_.getAddress() > 0; }
    InodeAddress getParentAddress() const noexcept { return parent_; }
    Inode getParent() const { return parent_.readInode(); }
    void setParent(const InodeAddress& parent) noexcept { parent_ = parent; }
    void setParent(const Inode& parent) noexcept { parent_ = parent.getAddress(); }

    bool hasNext() const noexcept { return next_.getAddress() > 0; }
    InodeAddress getNextAddress() const noexcept { return next_; }
    Inode getNext() const { return next_.readInode(); }
    void setNext(const InodeAddress& next) noexcept { next_ = next; }
    void setNext(const Inode& next) noexcept { next_ = next.getAddress(); }

    off_t getLength() const noexcept { return length_; }
    void setLength(off_t length) noexcept { length_ = length; }

    time_t getAccessTime() const noexcept { return atime_; }
    time_t getCreationTime() const noexcept { return ctime_; }

    mode_t getMode() const noexcept { return mode_; }

    gid_t getGid() const noexcept { return gid_; }
    uid_t getUid() const noexcept { return uid_; }

    bool save() const noexcept;
    void clear() noexcept;

    explicit operator bool() const noexcept {
        return addr_.getAddress() > 0;
    }

    explicit operator InodeAddress() const noexcept {
        return addr_;
    }

    explicit operator off_t() const noexcept {
        return addr_.getAddress();
    }

private:
    InodeAddress addr_;
    u8 type_;
    char filename_[128];

    InodeAddress parent_;
    InodeAddress next_;

    off_t length_;

    time_t atime_;
    time_t ctime_;

    mode_t mode_;

    gid_t gid_;
    uid_t uid_;
};

#endif //RTFS_INODE_H


#include <fuse3/fuse.h>
#include <cstdio>
#include <stdexcept>
#include <memory>

#include "rtfs/instance.h"
#include "rtfs/internal.h"
#include "rtfs/file.h"
#include "rtfs/folder.h"

#include "log.h"

using namespace std;



// Declarations of internal file system functions
INTERNAL int rtfs_getattr (const char*, struct stat*, struct fuse_file_info*);
INTERNAL int rtfs_readlink (const char*, char*, size_t);
INTERNAL int rtfs_mknod (const char*, mode_t, dev_t);
INTERNAL int rtfs_mkdir (const char*, mode_t);
INTERNAL int rtfs_unlink (const char*);
INTERNAL int rtfs_rmdir (const char*);
INTERNAL int rtfs_symlink (const char*, const char*);
INTERNAL int rtfs_rename (const char*, const char*, unsigned int);
INTERNAL int rtfs_link (const char*, const char*);
INTERNAL int rtfs_chmod (const char*, mode_t, struct fuse_file_info*);
INTERNAL int rtfs_chown (const char*, uid_t, gid_t, struct fuse_file_info*);
INTERNAL int rtfs_truncate (const char*, off_t, struct fuse_file_info*);
INTERNAL int rtfs_open (const char*, struct fuse_file_info*);
INTERNAL int rtfs_read (const char*, char*, size_t, off_t, struct fuse_file_info*);
INTERNAL int rtfs_write (const char*, const char*, size_t, off_t, struct fuse_file_info*);
INTERNAL int rtfs_statfs (const char*, struct statvfs*);
INTERNAL int rtfs_flush (const char*, struct fuse_file_info*);
INTERNAL int rtfs_release (const char*, struct fuse_file_info*);
INTERNAL int rtfs_fsync (const char*, int, struct fuse_file_info*);
INTERNAL int rtfs_opendir (const char*, struct fuse_file_info*);
INTERNAL int rtfs_readdir (const char*, void*, fuse_fill_dir_t, off_t, struct fuse_file_info*, enum fuse_readdir_flags);
INTERNAL int rtfs_releasedir (const char*, struct fuse_file_info*);
INTERNAL int rtfs_fsyncdir (const char*, int, struct fuse_file_info*);
INTERNAL void* rtfs_init (struct fuse_conn_info*, struct fuse_config*);
INTERNAL void rtfs_destroy (void*);
INTERNAL int rtfs_access (const char*, int);
INTERNAL int rtfs_create (const char*, mode_t, struct fuse_file_info*);
INTERNAL int rfts_fallocate (const char*, int, off_t, off_t, struct fuse_file_info*);

RtfsOperations::RtfsOperations() {
    // Assign function pointers
    operations_.getattr = rtfs_getattr;
    operations_.readlink = rtfs_readlink;
    operations_.mknod = rtfs_mknod;;// deprecated
    operations_.mkdir = rtfs_mkdir;
    operations_.unlink = rtfs_unlink;
    operations_.rmdir = rtfs_rmdir;
    operations_.symlink = rtfs_symlink;
    operations_.rename = rtfs_rename;
    operations_.link = rtfs_link;
    operations_.chmod = rtfs_chmod;
    operations_.chown = rtfs_chown;
    operations_.truncate = rtfs_truncate;
    operations_.open = rtfs_open;
    operations_.read = rtfs_read;
    operations_.write = rtfs_write;
    operations_.statfs = rtfs_statfs;
    operations_.flush = rtfs_flush;
    operations_.release = rtfs_release;
    operations_.fsync = rtfs_fsync;
    operations_.opendir = rtfs_opendir;
    operations_.readdir = rtfs_readdir;
    operations_.releasedir = rtfs_releasedir;
    operations_.fsyncdir = rtfs_fsyncdir;
    operations_.init = rtfs_init;
    operations_.destroy = rtfs_destroy;
    operations_.access = rtfs_access;
    operations_.create = rtfs_create;
    operations_.fallocate = rfts_fallocate;
}

int rtfs_getattr(const char* filename, struct stat* buffer, struct fuse_file_info* fi) {
    try {
        RtfsInstance* instance = RtfsInstance::getInstance();
        shared_ptr<RtfsBlock> block;
        if (fi->fh > 0) {
            block = instance->getOpen(fi->fh);
        } else {
            InodeAddress addr = instance->getTree()->getAddress(filename);
            block = shared_ptr<RtfsBlock>(RtfsBlock::readFromDisk(addr));
        }

        buffer->st_mode = block->getInode().getMode();
        buffer->st_gid = block->getInode().getGid();
        buffer->st_uid = block->getInode().getUid();
        buffer->st_size = block->getSize();
        buffer->st_nlink = 0;
        buffer->st_blksize = (instance->getSuperblock().getBlockSize() - sizeof(Inode));
        buffer->st_atim.tv_sec = block->getInode().getAccessTime();
        buffer->st_ctim.tv_sec = block->getInode().getCreationTime();

        return ERR_SUCCESS;
    } catch(exception& ex) {
        Log::getInstance() << ex;
        return ERR_ACTION_FAILED;
    }

}

int rtfs_readlink(const char *, char *, size_t) {
    // FS does not support links
    return ERR_NOT_IMPLEMENTED;
}

int rtfs_mknod(const char *, mode_t, dev_t) {
    // Deprecated, should not be called at all
    return ERR_NOT_IMPLEMENTED;
}

int rtfs_mkdir(const char* name, mode_t) {
    return 0;
}

int rtfs_unlink(const char* name) {
    return 0;
}

int rtfs_rmdir(const char* name) {
    return 0;
}

int rtfs_symlink(const char *, const char *) {
    // FS does not support links
    return ERR_NOT_IMPLEMENTED;
}

int rtfs_rename(const char* from, const char* to, unsigned int flags) {
    if (flags) {
        return ERR_NOT_IMPLEMENTED;
    }

    RtfsInstance* instance = RtfsInstance::getInstance();
    InodeAddress addr = instance->getTree()->getAddress(from);
    if (instance->getTree()->exists(to)) {
        Log::getInstance() << "Rename failed. From: " << string(from) << " - To: " << string(to) << Log::newLine();
        return ERR_ACTION_FAILED;
    }

    auto block = shared_ptr<RtfsBlock>(RtfsBlock::readFromDisk(addr));
    if (block->rename(to)) {
        return ERR_SUCCESS;
    }

    return ERR_ACTION_FAILED;
}

int rtfs_link(const char *, const char *) {
    // FS does not support links
    return ERR_NOT_IMPLEMENTED;
}

int rtfs_chmod(const char* path, mode_t mode, struct fuse_file_info* fi) {
    RtfsInstance* instance = RtfsInstance::getInstance();
    try {
        shared_ptr<RtfsBlock> block;
        if (fi->fh > 0) {
            block = instance->getOpen(fi->fh);
        } else {
            InodeAddress addr = instance->getTree()->getAddress(path);

            if (instance->isOpen(addr)) {
                block = instance->getOpen(addr);
            } else {
                block = shared_ptr<RtfsBlock>(RtfsBlock::readFromDisk(addr));
            }
        }

        if (block->updateMode(mode)) {
            return ERR_SUCCESS;
        }

        return ERR_ACTION_FAILED;
    } catch (std::exception& ex) {
        Log::getInstance() << ex;
        return ERR_GENERAL_ERROR;
    }
}

int rtfs_chown(const char* path, uid_t uid, gid_t gid, struct fuse_file_info* fi) {
    RtfsInstance* instance = RtfsInstance::getInstance();
    try {
        shared_ptr<RtfsBlock> block;
        if (fi->fh > 0) {
            block = instance->getOpen(fi->fh);
        } else {
            InodeAddress addr = instance->getTree()->getAddress(path);

            if (instance->isOpen(addr)) {
                block = instance->getOpen(addr);
            } else {
                block = shared_ptr<RtfsBlock>(RtfsBlock::readFromDisk(addr));
            }
        }

        if (block->updateOwner(uid, gid)) {
            return ERR_SUCCESS;
        }

        return ERR_ACTION_FAILED;
    } catch (std::exception& ex) {
        Log::getInstance() << ex;
        return ERR_GENERAL_ERROR;
    }
}

int rtfs_truncate(const char* path, off_t size, struct fuse_file_info* fi) {
    RtfsInstance* instance = RtfsInstance::getInstance();
    try {
        shared_ptr<RtfsBlock> block;
        if (fi->fh > 0) {
            block = instance->getOpenFile(fi->fh);
        } else {
            InodeAddress addr = instance->getTree()->getAddress(path);

            if (instance->isOpen(addr)) {
                block = instance->getOpen(addr);
            } else {
                // Will be closed immedatly after resize
                block = shared_ptr<RtfsBlock>(RtfsBlock::readFromDisk(addr));
            }
        }

        if (block->getType() == TYPE_FILE) {
            auto* file = dynamic_cast<RtfsFile*>(block.get());

            if (file->resize(size)) {
                return ERR_SUCCESS;
            }
        }

        return ERR_ACTION_FAILED;
    } catch (std::exception& ex) {
        Log::getInstance() << ex;
        return ERR_GENERAL_ERROR;
    }
}

int rtfs_open(const char* name, struct fuse_file_info* fi) {
    if (RtfsInstance::getInstance()->openFile(name, fi)) {
        return ERR_SUCCESS;
    }
    return ERR_ACTION_FAILED;
}

int rtfs_read(const char* path, char* buffer, size_t length, off_t offset, struct fuse_file_info* fi) {
    return 0;
}

int rtfs_write(const char* path, const char* data, size_t length, off_t offset, struct fuse_file_info* fi) {
    return 0;
}

int rtfs_statfs(const char *, struct statvfs *) {
    // @todo could be useful in future
    return ERR_NOT_IMPLEMENTED;
}

int rtfs_flush(const char*, struct fuse_file_info *) {
    RtfsInstance::getInstance()->getFile().flush();
    return 0;
}

int rtfs_release(const char* path, struct fuse_file_info* fi) {
    RtfsInstance* instance = RtfsInstance::getInstance();
    if (fi->fh > 0 && instance->close(fi->fh)) {
        return ERR_SUCCESS;
    } else {
        InodeAddress addr = instance->getTree()->getAddress(path);
        if (instance->isOpen(addr) && instance->close(instance->getOpen(addr))) {
            return ERR_SUCCESS;
        }
        Log::getInstance() << "No file handle and no open path name provided." << Log::newLine();
    }
    return ERR_ACTION_FAILED;
}

int rtfs_fsync(const char* path, int, struct fuse_file_info* fi) {
    return rtfs_flush(path, fi);
}

int rtfs_opendir(const char* name, struct fuse_file_info* fi) {
    RtfsInstance* instance = RtfsInstance::getInstance();
    if (instance->openFolder(name, fi)) {
        return ERR_SUCCESS;
    }

    return ERR_ACTION_FAILED;
}

int rtfs_readdir(const char* path, void* buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi, enum fuse_readdir_flags) {
    if (fi->fh <= 0) {
        return ERR_INVALID_HANLE;
    }
    try {
        RtfsInstance *instance = RtfsInstance::getInstance();
        auto block = instance->getOpenFolder(fi->fh);
        if (!block) {
            Log::getInstance() << "Cannot find handle " << fi->fh << " for folder: " << path << Log::newLine();
            return ERR_ACTION_FAILED;
        }

        auto* folder = dynamic_cast<RtfsFolder*>(block.get());
        auto func = [=](string name, off_t offset) -> bool {
            // maybe remove FUSE_FILL_DIR_PLUS to get it working
            return filler(buffer, name.c_str(), NULL, offset, FUSE_FILL_DIR_PLUS) == 0;
        };

        if (folder->readFolder(offset, func)) {
            return ERR_SUCCESS;
        }

        return ERR_ACTION_FAILED;
    } catch (exception& ex) {
        Log::getInstance() << ex;
        return ERR_GENERAL_ERROR;
    }
}

int rtfs_releasedir(const char* name, struct fuse_file_info* fi) {
    RtfsInstance* instance = RtfsInstance::getInstance();
    if (fi->fh > 0 && instance->close(fi->fh)) {
        return ERR_SUCCESS;
    } else {
        InodeAddress addr = instance->getTree()->getAddress(name);
        if (instance->isOpen(addr) && instance->close(instance->getOpen(addr))) {
            return ERR_SUCCESS;
        }
        Log::getInstance() << "No file handle and no open path name provided." << Log::newLine();
    }
    return ERR_ACTION_FAILED;
}

int rtfs_fsyncdir(const char *, int, struct fuse_file_info *) {
    RtfsInstance::getInstance()->getFile().flush();
    return ERR_SUCCESS;
}

void* rtfs_init(struct fuse_conn_info*, struct fuse_config* config) {
    return RtfsInstance::getInstance()->init(config);
}

void rtfs_destroy(void*) {
    RtfsInstance* instance = RtfsInstance::getInstance();
    // Save tree
    instance->getTree()->save(instance->getFile(), sizeof(Superblock));

    // Destroy instance structure
    delete instance;
}

int rtfs_access(const char *, int) {
    // Intentionally does nothing
    return 0;
}

int rtfs_create(const char* path, mode_t mode, struct fuse_file_info* fi) {


    return 0;
}

int rfts_fallocate(const char *, int, off_t, off_t, struct fuse_file_info *) {
    return 0;
}

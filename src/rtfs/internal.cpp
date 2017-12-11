
#include <fuse.h>

#include "rtfs/instance.h"
#include "rtfs/internal.h"


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

/*
INTERNAL int rtfs_setxattr (const char*, const char*, const char*, size_t, int);
INTERNAL int rtfs_getxattr (const char*, const char*, char*, size_t);
INTERNAL int rtfs_listxattr (const char*, char*, size_t);
INTERNAL int rtfs_removexattr (const char*, const char*);

.setxattr = rtfs_setxattr,
.getxattr = rtfs_getxattr,
.listxattr = rtfs_listxattr,
.removexattr = rtfs_removexattr,
*/

/**
 * Set static operations struct
 * Struct contains function pointer to concrete file system functions
 */
struct fuse_operations RtfsOperations::operations_ = {
        .getattr = rtfs_getattr,
        .readlink = rtfs_readlink,
        .mknod = rtfs_mknod, // deprecated
        .mkdir = rtfs_mkdir,
        .unlink = rtfs_unlink,
        .rmdir = rtfs_rmdir,
        .symlink = rtfs_symlink,
        .rename = rtfs_rename,
        .link = rtfs_link,
        .chmod = rtfs_chmod,
        .chown = rtfs_chown,
        .truncate = rtfs_truncate,
        .open = rtfs_open,
        .read = rtfs_read,
        .write = rtfs_write,
        .statfs = rtfs_statfs,
        .flush = rtfs_flush,
        .release = rtfs_release,
        .fsync = rtfs_fsync,
        .opendir = rtfs_opendir,
        .readdir = rtfs_readdir,
        .releasedir = rtfs_releasedir,
        .fsyncdir = rtfs_fsyncdir,
        .init = rtfs_init,
        .destroy = rtfs_destroy,
        .access = rtfs_access,
        .create = rtfs_create,
        .fallocate = rfts_fallocate
};

int rtfs_getattr(const char *, struct stat *, struct fuse_file_info *) {
    return 0;
}

int rtfs_readlink(const char *, char *, size_t) {
    return 0;
}

int rtfs_mknod(const char *, mode_t, dev_t) {
    // Deprecated, should not be called at all
    return ERR_NOT_IMPLEMENTED;
}

int rtfs_mkdir(const char *, mode_t) {
    return 0;
}

int rtfs_unlink(const char *) {
    return 0;
}

int rtfs_rmdir(const char *) {
    return 0;
}

int rtfs_symlink(const char *, const char *) {
    return 0;
}

int rtfs_rename(const char *, const char *, unsigned int) {
    return 0;
}

int rtfs_link(const char *, const char *) {
    return 0;
}

int rtfs_chmod(const char *, mode_t, struct fuse_file_info *) {
    return 0;
}

int rtfs_chown(const char *, uid_t, gid_t, struct fuse_file_info *) {
    return 0;
}

int rtfs_truncate(const char *, off_t, struct fuse_file_info *) {
    return 0;
}

int rtfs_open(const char* name, struct fuse_file_info* fi) {
    if (RtfsInstance::getInstance()->openFile(string(name), fi)) {
        return ERR_SUCCESS;
    }
    return ERR_GENERAL_ERROR;
}

int rtfs_read(const char *, char *, size_t, off_t, struct fuse_file_info *) {
    return 0;
}

int rtfs_write(const char *, const char *, size_t, off_t, struct fuse_file_info *) {
    return 0;
}

int rtfs_statfs(const char *, struct statvfs *) {
    return 0;
}

int rtfs_flush(const char *, struct fuse_file_info *) {
    return 0;
}

int rtfs_release(const char *, struct fuse_file_info *) {
    return 0;
}

int rtfs_fsync(const char *, int, struct fuse_file_info *) {
    return 0;
}

int rtfs_opendir(const char *, struct fuse_file_info *) {
    return 0;
}

int rtfs_readdir(const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *, enum fuse_readdir_flags) {
    return 0;
}

int rtfs_releasedir(const char *, struct fuse_file_info *) {
    return 0;
}

int rtfs_fsyncdir(const char *, int, struct fuse_file_info *) {
    return 0;
}

void* rtfs_init(struct fuse_conn_info*, struct fuse_config* config) {
    return RtfsInstance::getInstance()->init(config);
}

void rtfs_destroy(void *) {
    // Intentionally does nothing
}

int rtfs_access(const char *, int) {
    return 0;
}

int rtfs_create(const char *, mode_t, struct fuse_file_info *) {
    return 0;
}

int rfts_fallocate(const char *, int, off_t, off_t, struct fuse_file_info *) {
    return 0;
}

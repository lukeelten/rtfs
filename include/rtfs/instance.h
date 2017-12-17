
#ifndef RTFS_INSTANCE_H
#define RTFS_INSTANCE_H

#include <fuse.h>
#include <cstdio>
#include <string>
#include <atomic>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <sys/stat.h>

#include "superblock.h"
#include "file.h"
#include "folder.h"

using std::string;
using std::unordered_map;
using std::shared_ptr;

typedef uint64_t FileDescriptor;
class FileHandler; // Forward declaration

class RtfsInstance {

public:
    RtfsInstance(const string& file_);
    ~RtfsInstance(); // Default is fine for us

    // No copy & No move
    RtfsInstance(const RtfsInstance& ) = delete;
    RtfsInstance& operator = (const RtfsInstance& ) = delete;
    RtfsInstance(RtfsInstance&& ) = delete;
    RtfsInstance& operator = (RtfsInstance&& ) = delete;

    // Init FS; Called by fuse
    RtfsInstance* init (struct fuse_config* config_) noexcept;

    bool openFile(string filename, struct fuse_file_info* fi);
    bool openFolder(string name, struct fuse_file_info* fi);

    const Superblock& getSuperblock() const noexcept { return superblock; }
    const Inode& getInode() const noexcept { return root; }

    static RtfsInstance* getInstance() noexcept {
        return static_cast<RtfsInstance*>(fuse_get_context()->private_data);
    }

    const FileHandler& getFile() const noexcept {
        // In practice not really const, but what ever ....
        return file;
    }

    shared_ptr<RtfsFile> getOpenFile(FileDescriptor fd);
    shared_ptr<RtfsFolder> getOpenFolder(FileDescriptor fd);
    shared_ptr<RtfsBlock> getOpen(FileDescriptor fd);
    shared_ptr<RtfsBlock> getOpen(InodeAddress addr);

    bool isOpen(InodeAddress addr) const { return openAddresses.find(addr) != openAddresses.end();}


private:
    FileDescriptor getNextDescriptor();

    FileHandler file;

    Superblock superblock;
    Inode root;

    // Will be handled by fuse, DO NOT DELETE
    fuse_config* config;

    unordered_map<FileDescriptor, shared_ptr<RtfsFile>> openFiles;
    unordered_map<InodeAddress, FileDescriptor> openAddresses;
    unordered_map<FileDescriptor, shared_ptr<RtfsFolder>> openFolders;

    std::atomic<FileDescriptor> counter;
};

class FileHandler {
public:
    FileHandler(const string& filename_);
    ~FileHandler() = default;

    // No copy & move; In theory, move would be fine;
    // Cannot copy because of mutex
    FileHandler(const FileHandler& ) = delete;
    FileHandler& operator = (const FileHandler& ) = delete;
    FileHandler(FileHandler&& ) = delete;
    FileHandler& operator = (FileHandler&& ) = delete;

    void open() {
        FILE* handle = fopen(filename.c_str(), "rb+");
        if (handle == NULL) {
            throw std::runtime_error("Cannot open file");
        }
        file = std::shared_ptr(handle, [](FILE* fp) {fflush(fp); fclose(fp);});
    }
    const string& getFilename() const noexcept { return filename; }
    void flush() const noexcept { fflush(file.get()); }

    bool exists() const noexcept {
        struct stat stats;
        int exists = stat(filename.c_str(), &stats);

        return (exists == 0);
    }

    template<class T>
    bool read(T* buffer) const noexcept {
        return read<T>(buffer, 0, SEEK_CUR);
    }

    template<class T>
    bool read(T* buffer, off_t position, int flags = SEEK_SET) const noexcept {
        std::lock_guard<std::mutex> guard(lock);
        if (buffer == NULL || fseek(file.get(), position, flags) != 0) {
            // @todo log ferror
            return false;
        }

        return fread(buffer, sizeof(T), 1, file.get()) == sizeof(T);
    }

    template<class T>
    bool write(const T* data) const noexcept {
        return write<T>(data, 0, SEEK_CUR);
    }

    template<class T>
    bool write(const T* data, off_t position, int flags = SEEK_SET) const noexcept {
        std::lock_guard<std::mutex> guard(lock);
        if (data == NULL || fseek(file.get(), position, flags) != 0) {
            // @todo log ferror
            return false;
        }

        return fwrite(data, sizeof(T), 1, file.get()) == sizeof(T);
    }

    bool setPosition(off_t position, int flags = SEEK_SET) const noexcept {
        std::lock_guard<std::mutex> guard(lock);
        return fseek(file.get(), position, flags) == 0;
    }

private:
    const string filename;
    std::shared_ptr<FILE> file;
    mutable std::mutex lock;
};



#endif //RTFS_INSTANCE_H

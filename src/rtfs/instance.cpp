
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <sys/stat.h>

#include "rtfs/instance.h"

using namespace std;

bool file_exists(const string& filename) noexcept {
    struct stat stats;
    int exists = stat(filename.c_str(), &stats);

    return (exists == 0);
}


RtfsInstance::RtfsInstance(string file_) : filename(move(file_)), openFiles() {
    if (!file_exists(filename)) {
        throw runtime_error("File does not exist.");
    }
    file = nullptr;
}

RtfsInstance::~RtfsInstance() {
    if (file != nullptr) {
        lock_guard<mutex> guard(lock);

        fflush(file);
        fclose(file);
        file = nullptr;
    }
}

RtfsInstance* RtfsInstance::init(struct fuse_config *config_) noexcept {
    config = config_;
    file = fopen(filename.c_str(), "rb+");

    if (file == NULL) {
        throw runtime_error("Cannot open FS image.");
    }

    return this;
}

bool RtfsInstance::openFile(string filename, struct fuse_file_info* fi) {

    InodeAddress addr; // Find in tree

    if (openFiles.find(addr) != openFiles.end()) {
        // File already open
        return false;
    }

    fi->fh = static_cast<uint64_t>(addr.getAddress());
    openFiles.insert(addr);

    return true;
}
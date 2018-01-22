#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <sys/stat.h>
#include <fuse3/fuse.h>

#include "rtfs.h"

using namespace std;


int main(int argc, char* argv[]) {

    off_t size = DEFAULT_SIZE; // 100 MB
    off_t block_size = DEFAULT_BLOCKSIZE; // 4 KB
    string filename = string(DEFAULT_FILENAME);

    {
        struct stat stats;
        int exists = stat(filename.c_str(), &stats);

        if (exists != 0) {
            Formatter formatter;
            formatter.setFilename(filename);
            formatter.setBlockSize(block_size);
            formatter.setFilesystemSize(size);

            formatter.format();
        }
    }

    RtfsOperations op;
    struct fuse_operations rtfs_operations = op.getOperations();
    auto* rtfs = new RtfsInstance(filename);
    int fuse_stat = fuse_main(argc, argv,&rtfs_operations, rtfs);


    RtfsInstance* inst = RtfsInstance::getInstance();
    off_t treeAddr = 0 + sizeof(Superblock);

    /*
    const auto& filehandler = inst->getFile();
    size_t i = 410;
    filehandler.write<size_t>(&i, treeAddr);
     */


    // Will return fast and turn over control to fuse
    // Instance must not be deleted
    return fuse_stat;
}
#include <iostream>
#include <string>
#include <regex>
#include <memory>
#include <sys/stat.h>
#include <fuse.h>

#include "rtfs.h"

using namespace std;

void printHelp() {

}

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

    struct fuse_operations rtfs_operations = RtfsOperations::getOperations();
    RtfsInstance* rtfs = new RtfsInstance(filename);
    int fuse_stat = fuse_main(argc, argv,&rtfs_operations, rtfs);

    return fuse_stat;
}
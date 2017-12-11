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

    /*
    if (argc > 1) {
        string arg (argv[1]);
        if (arg == "--help") {
            printHelp();
            return 0;
        }

        filename = move(arg);
        argc--;
    }

    if (argc > 1) {
        string arg2(argv[2]);
        regex reg("([0-9]+)(k|M|G)?");
        smatch match;

        if (regex_search(arg2, match, reg)) {
            if (match.size() == 2) {
                size = stol(match.str(1));
            } else if (match.size() == 3) {
                off_t base = stol(match.str(1));
                string exp = match.str(2);

                if (exp == "k") {
                    base *= 1024;
                } else if (exp == "M") {
                    base *= 1024 * 1024;
                } else if (exp == "G") {
                    base *= 1024 * 1024 * 1024;
                }

                size = base;
            }
        }
        argc--;
    }

    if (argc > 1) {
        string arg3(argv[3]);
        block_size = stol(arg3);
        argc--;
    } */

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
    unique_ptr<RtfsInstance> rtfs = make_unique<RtfsInstance>(filename);
    int fuse_stat = fuse_main(argc, argv,&rtfs_operations, rtfs.get());

    return fuse_stat;
}
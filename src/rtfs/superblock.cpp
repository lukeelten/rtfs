
#include <cstdio>
#include <stdexcept>

#include "rtfs/instance.h"
#include "rtfs/superblock.h"


using namespace std;


Superblock Superblock::readFromDisk() {
    RtfsInstance* instance = RtfsInstance::getInstance();

    Superblock superblock;
    if (instance->getFile().read<Superblock>(&superblock, 0)) {
        Log::getInstance() << superblock;
        return superblock;
    }

    throw runtime_error("Cannot read superblock");
}
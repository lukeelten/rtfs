
#include <cstdio>
#include <stdexcept>

#include "rtfs/instance.h"
#include "rtfs/superblock.h"


using namespace std;


Superblock Superblock::readFromDisk() {
    RtfsInstance* instance = RtfsInstance::getInstance();

    Superblock superblock;
    if (instance->getFile().read(&superblock)) {
        return move(superblock);
    }

    throw runtime_error("Cannot read superblock");
}
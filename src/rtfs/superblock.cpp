
#include <cstdio>
#include <stdexcept>

#include "rtfs/instance.h"
#include "rtfs/superblock.h"


using namespace std;


Superblock Superblock::readFromDisk() {
    RtfsInstance* instance = RtfsInstance::getInstance();

    fseek(instance->getFile(), 0, SEEK_SET);
    Superblock superblock;
    if (fread(&superblock, sizeof(Superblock), 1, instance->getFile()) == sizeof(Superblock)) {
        return move(superblock);
    }

    throw runtime_error("Cannot read superblock");
}
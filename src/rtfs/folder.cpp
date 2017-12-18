#include <vector>
#include <functional>
#include <string>


#include "rtfs/folder.h"
#include "log.h"


using namespace std;

bool RtfsFolder::readFolder(off_t start, function<bool(string, off_t)> filler) {
    Log::getInstance() << "Read dir " << getInode().getFilename() << " with offset: " << start << Log::newLine();


    // First two entries are current and parent folder
    if (start == 0) {
        filler(".", ++start);
    }

    bool hasParent = getInode().hasParent();
    if (start == 1 && hasParent) { // Only root does not have a parent
        filler("..", ++start);
    }

    off_t cnt = 0;
    for (auto it : children) {
        if (cnt < (start - ((hasParent) ? 2 : 1))) {
            continue;
        }

        auto inode = it.readInode();
        if (!filler(inode.getFilename(), ++start)) {
            // Filler returns false when buffer is full
            // Restarts by FUSE with corrent offset
            break;
        }
        cnt++;
    }

    return true;
}

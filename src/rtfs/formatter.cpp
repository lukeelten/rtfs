
#include <string>
#include <stdexcept>
#include <cstdio>
#include <unistd.h>
#include <iostream>

#include "rtfs.h"
#include "rtfs/formatter.h"

using namespace std;


Formatter::~Formatter() {
    if (fp) {
        fflush(fp);
        fclose(fp);
    }
}

void Formatter::format() {
    calculateSizes();

    allocateFile();

    writeSuperblock();

    fseek(fp, superblock.treeSize, SEEK_CUR);
    Inode root = Inode::initEmpty(superblock.root);
    root.setType(TYPE_DIR);
    root.setFilename("/");
    writeInode(root);

    for (off_t i = 0; i < superblock.numInodes; i++)  {
        off_t addr = superblock.root + superblock.blockSize + (i * superblock.blockSize);
        Inode inode = Inode::initEmpty(addr);
        writeInode(inode);
    }

    fflush(fp);
    fclose(fp);
    fp = nullptr;
}

void Formatter::calculateSizes() noexcept {
    superblock.treeSize = (size_t) ((double) superblock.totalSize * 0.05d);
    off_t newSize = superblock.totalSize - superblock.treeSize - sizeof(Superblock);

    superblock.numInodes = newSize / superblock.blockSize;
    superblock.totalSize = superblock.numInodes * superblock.blockSize;

    superblock.root = (sizeof(Superblock) + superblock.treeSize);
    superblock.version = RTFS_VERSION;
}

void Formatter::allocateFile() {
    fp = fopen(filename_.c_str(), "wb+");
    if (!fp) {
        throw runtime_error("Cannot allocate file");
    }

    ftruncate(fileno(fp), superblock.totalSize);
    fflush(fp);
}

void Formatter::writeSuperblock() {
    cout << dec << "Version: " << superblock.version << " - " << hex << superblock.version << endl;
    cout << dec << "Block Size: " << superblock.blockSize << " - " << hex << superblock.blockSize << endl;
    cout << dec << "Num Inodes: " << superblock.numInodes << " - " << hex << superblock.numInodes << endl;
    cout << dec << "Total Size: " << superblock.totalSize << " - " << hex << superblock.totalSize << endl;
    cout << dec << "Tree Size: " << superblock.treeSize << " - " << hex << superblock.treeSize << endl;
    cout << dec << "Root Addr: " << superblock.root.getAddress() << " - " << hex << superblock.root.getAddress() << endl;

    fseek(fp, 0, SEEK_SET);
    fwrite(&superblock, sizeof(Superblock), 1, fp);
}

void Formatter::writeInode(Inode inode) {
    fseek(fp, inode.getAddress().getAddress(), SEEK_SET);
    fwrite(&inode, sizeof(Inode), 1, fp);
}
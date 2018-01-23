
#include <string>
#include <stdexcept>
#include <cstdio>
#include <unistd.h>
#include <iostream>

#include "rtfs.h"
#include "rtfs/formatter.h"

using namespace std;


Formatter::~Formatter() {
    if (file.is_open()) {
        file.flush();
        file.close();
    }
}

void Formatter::format() {
    calculateSizes();

    allocateFile();

    writeSuperblock();

    Inode root = Inode::initEmpty(superblock.root);
    root.setType(TYPE_DIR);
    root.setFilename("/");

    BTree tree;
    tree.insert("/", root.getAddress());
    // Save tree

    writeInode(root);

    for (off_t i = 0; i < superblock.numInodes; i++)  {
        off_t addr = static_cast<off_t>(superblock.root) + superblock.blockSize + (i * superblock.blockSize);
        writeInode(Inode::initEmpty(InodeAddress(addr)));
    }

    file.flush();
}

void Formatter::calculateSizes() noexcept {
    superblock.treeSize = (size_t) ((double) superblock.totalSize * 0.05d);
    off_t newSize = superblock.totalSize - superblock.treeSize - sizeof(Superblock);

    superblock.numInodes = newSize / superblock.blockSize;
    superblock.totalSize = superblock.numInodes * superblock.blockSize;

    superblock.root = InodeAddress(sizeof(Superblock) + superblock.treeSize);
}

void Formatter::allocateFile() {
    FILE* fp = fopen(filename_.c_str(), "wb");
    if (!fp) {
        throw runtime_error("Cannot allocate file");
    }

    ftruncate(fileno(fp), superblock.totalSize);
    fflush(fp);
    fclose(fp);

    file.open(filename_, ios::in | ios::out | ios::binary);
}

void Formatter::writeSuperblock() {
    cout << dec << "Block Size: " << superblock.blockSize << " - " << hex << superblock.blockSize << endl;
    cout << dec << "Num Inodes: " << superblock.numInodes << " - " << hex << superblock.numInodes << endl;
    cout << dec << "Total Size: " << superblock.totalSize << " - " << hex << superblock.totalSize << endl;
    cout << dec << "Tree Size: " << superblock.treeSize << " - " << hex << superblock.treeSize << endl;
    cout << dec << "Root Addr: " << superblock.root.getAddress() << " - " << hex << superblock.root.getAddress() << endl;

    Log::getInstance() << superblock;

    Superblock* pointer = &superblock;

    file.seekp(0);
    file.write((char*)pointer, sizeof(Superblock));
    file.flush();
}

void Formatter::writeInode(Inode inode) {
    Inode* pointer = &inode;
    file.seekp(inode.getAddress().getAddress());
    file.write((char*) pointer, sizeof(Inode));
    file.flush();
}


#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "rtfs/superblock.h"
#include "rtfs.h"
#include "log.h"

using namespace std;


Log &Log::getInstance() {
    // Meyer´s singleton
    static Log instance;
    return instance;
}

Log &Log::operator<<(const string &str) {
    file << str;
    file.flush();
    return *this;
}

Log &Log::operator<<(off_t data) {
    file << data;
    file.flush();
    return *this;
}

Log &Log::operator<<(InodeAddress addr) {
    file << "Inode Address: " << addr.getAddress();
    file.flush();
    return *this;
}

Log &Log::operator<< (const Inode &inode) {
    file << "Inode:" << newLine();
    file << tab() << "Address: " << inode.getAddress().getAddress() << newLine();
    file << tab() << "Type: " << inode.getType() << newLine();
    file << tab() << "Name: " << inode.getFilename() << newLine();
    file << tab() << "Parent: " << inode.getParentAddress().getAddress() << newLine();
    file << tab() << "Next: " << inode.getNextAddress().getAddress() << newLine();
    file << newLine();
    file.flush();
    return *this;
}

Log &Log::operator<< (const RtfsFile& rtfsFile) {
    file << "File:" << newLine();
    file << tab() << "Address: " << rtfsFile.getInode().getAddress().getAddress() << newLine();
    file << tab() << "Filename: " << rtfsFile.getInode().getFilename() << newLine();
    file << tab() << "Size: " << rtfsFile.getSize() << newLine();
    file << newLine();
    file.flush();
    return *this;
}

Log &Log::operator<<(const RtfsFolder& folder) {
    file << "Folder:" << newLine();
    file << tab() << "Address: " << folder.getInode().getAddress().getAddress() << newLine();
    file << tab() << "Name: " << folder.getInode().getFilename() << newLine();
    file << tab() << "SubFolder: " << folder.numChildren() << newLine();
    file << newLine();
    file.flush();
    return *this;
}

Log &Log::operator<<(const exception &ex) {
    file << "Exception: " << newLine();
    file << tab() << "Type: " << typeid(ex).name() << newLine();
    file << tab() << "Message: " << ex.what() << newLine();
    file << newLine();
    file.flush();
    return *this;
}

Log::Log() : file("/home/lukeelten/rtfs.log", ofstream::out | ofstream::trunc) {
    if (!file.is_open()) {
        throw runtime_error("Cannot open log file");
    }
}

Log::~Log() {
    file.close();
}

Log &Log::operator<<(const char *str) {
    file << str;
    file.flush();
    return *this;
}

Log &Log::operator<<(const Superblock& superblock) {
    file << dec << "Block Size: " << superblock.getBlockSize() << " - " << hex << superblock.getBlockSize() << newLine();
    file << dec << "Num Inodes: " << superblock.getNumInodes() << " - " << hex << superblock.getNumInodes()<< newLine();
    file << dec << "Total Size: " << superblock.getTotalSize() << " - " << hex << superblock.getTotalSize() << newLine();
    file << dec << "Tree Size: " << superblock.getTreeSize() << " - " << hex << superblock.getTreeSize() << newLine();
    file << dec << "Root Addr: " << superblock.getRoot().getAddress() << " - " << hex << superblock.getRoot().getAddress() << newLine();
    file << dec << newLine();
    file.flush();

    return *this;
}

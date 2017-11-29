#ifndef RTFS_FORMATTER_H
#define RTFS_FORMATTER_H

#include <string>
#include <cstdio>

using std::string;

class Inode;

class Formatter {
public:
    Formatter() : filename_(), fp(nullptr), superblock() {}
    ~Formatter();

    // Copy is useless
    Formatter(const Formatter& ) = delete;
    Formatter(Formatter&& ) = delete;
    Formatter& operator = (const Formatter& ) = delete;
    Formatter&operator = (Formatter&& ) = delete;


    void format();

    void setFilename(const string& filename) noexcept { filename_ = filename; }
    void setBlockSize(off_t blockSize) noexcept { superblock.blockSize = blockSize; }
    void setFilesystemSize(off_t size) noexcept { superblock.totalSize = size; }

private:
    void allocateFile();
    void calculateSizes() noexcept;

    void writeSuperblock();
    void writeInode(Inode inode);

    string filename_;

    FILE* fp;
    Superblock superblock;
};




#endif //RTFS_FORMATTER_H

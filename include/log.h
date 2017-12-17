
#ifndef RTFS_LOG_H
#define RTFS_LOG_H

#include <fstream>
#include <string>
#include <stdexcept>

#include "rtfs/inode.h"
#include "rtfs/inode_address.h"
#include "rtfs/file.h"
#include "rtfs/folder.h"

using std::string;
using std::ofstream;
using std::exception;

class Log {
public:
    static Log& getInstance();
    static string newLine() noexcept { return "\n"; } // Can be noexcept because string will be fully allocated on the stack
    static string tab() noexcept { return "\t"; }

    ~Log();
    Log(const Log& ) = delete;
    Log(Log && ) = delete;
    Log& operator = (const Log& ) = delete;
    Log& operator = (Log&& ) = delete;


    Log& operator << (const string& str);
    Log& operator << (off_t data);
    Log& operator << (InodeAddress addr);
    Log& operator << (const Inode& inode);

    Log& operator << (const RtfsFile& rtfsFile);
    Log& operator << (const RtfsFolder& folder);
    Log& operator << (const exception& ex);

private:
    Log();

    ofstream file;
};



#endif //RTFS_LOG_H

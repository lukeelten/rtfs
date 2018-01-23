
#ifndef RTFS_FILE_HANDLER_H
#define RTFS_FILE_HANDLER_H

#include <string>
#include <fstream>
#include <stdexcept>
#include <mutex>
#include <memory>

#include "../log.h"

using std::string;
using std::fstream;
using std::ios;

class FileHandler {
public:
    FileHandler(const string& filename_) : filename(filename_), file(filename_, ios::in | ios::out| ios::binary), lock() {};
    ~FileHandler() {
        file.flush();
        file.close();
    };

    // No copy & move; In theory, move would be fine;
    // Cannot copy because of mutex
    FileHandler(const FileHandler& ) = delete;
    FileHandler& operator = (const FileHandler& ) = delete;
    FileHandler(FileHandler&& ) = delete;
    FileHandler& operator = (FileHandler&& ) = delete;

    bool exists() const noexcept {
        struct stat stats;
        int exists = stat(filename.c_str(), &stats);

        return (exists == 0);
    }

    template<class T>
    bool read(T* buffer, off_t position, int flags = SEEK_SET) const {
        try {
            std::lock_guard<std::mutex> guard(lock);
            file.seekg(position);
            file.read((char*) buffer, sizeof(T));

            return true;
        } catch (exception ex) {
            Log::getInstance() << ex;
            return false;
        }
    }

    template<class T>
    bool write(const T* data, off_t position, int flags = SEEK_SET) const {
        try {
            std::lock_guard<std::mutex> guard(lock);
            file.seekp(position);
            file.write((char*) data, sizeof(T));
            file.flush();

            return true;
        } catch (exception ex) {
            Log::getInstance() << ex;
            return false;
        }
    }

private:
    const string filename;
    mutable fstream file;
    mutable std::mutex lock;
};

#endif //RTFS_FILE_HANDLER_H

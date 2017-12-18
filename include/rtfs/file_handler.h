
#ifndef RTFS_FILE_HANDLER_H
#define RTFS_FILE_HANDLER_H

#include <string>
#include <cstdio>
#include <stdexcept>
#include <mutex>
#include <memory>

#include "../log.h"

using std::string;

class FileHandler {
public:
    FileHandler(const string& filename_);
    ~FileHandler() = default;

    // No copy & move; In theory, move would be fine;
    // Cannot copy because of mutex
    FileHandler(const FileHandler& ) = delete;
    FileHandler& operator = (const FileHandler& ) = delete;
    FileHandler(FileHandler&& ) = delete;
    FileHandler& operator = (FileHandler&& ) = delete;

    void open() {
        FILE* handle = fopen(filename.c_str(), "rb+");
        if (handle == NULL) {
            throw std::runtime_error("Cannot open file");
        }
        file = std::shared_ptr<FILE>(handle, [](FILE* fp) {fflush(fp); fclose(fp);});
    }

    const string& getFilename() const noexcept { return filename; }
    void flush() const noexcept { fflush(file.get()); }

    bool exists() const noexcept {
        struct stat stats;
        int exists = stat(filename.c_str(), &stats);

        return (exists == 0);
    }

    template<class T>
    bool read(T* buffer) const noexcept {
        return read<T>(buffer, 0, SEEK_CUR);
    }

    template<class T>
    bool read(T* buffer, off_t position, int flags = SEEK_SET) const noexcept {
        std::lock_guard<std::mutex> guard(lock);
        if (buffer == NULL || fseek(file.get(), position, flags) != 0) {
            Log::getInstance() << "Error while reading file" << Log::newLine();
            Log::getInstance() << Log::tab() << ferror(file.get()) << Log::newLine();

            return false;
        }

        return fread(buffer, sizeof(T), 1, file.get()) == sizeof(T);
    }

    template<class T>
    bool write(const T* data) const noexcept {
        return write<T>(data, 0, SEEK_CUR);
    }

    template<class T>
    bool write(const T* data, off_t position, int flags = SEEK_SET) const noexcept {
        std::lock_guard<std::mutex> guard(lock);
        if (data == NULL || fseek(file.get(), position, flags) != 0) {
            Log::getInstance() << "Error while writing file" << Log::newLine();
            Log::getInstance() << Log::tab() << ferror(file.get()) << Log::newLine();
            return false;
        }

        return fwrite(data, sizeof(T), 1, file.get()) == sizeof(T);
    }

    bool setPosition(off_t position, int flags = SEEK_SET) const noexcept {
        std::lock_guard<std::mutex> guard(lock);
        return fseek(file.get(), position, flags) == 0;
    }

private:
    const string filename;
    std::shared_ptr<FILE> file;
    mutable std::mutex lock;
};

#endif //RTFS_FILE_HANDLER_H

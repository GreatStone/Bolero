#include "env_hdfs.h"
#include "leveldb/env.h"

#include "hdfs.h"

#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

#define debug_log printf("error %d\n",__LINE__);

namespace bolero{
    using namespace ::leveldb;
    static Status IOError(const std::string& context, int err_number) {
        return Status::IOError(context, strerror(err_number));
    }
    class HDFSReadableFile: virtual public SequentialFile,
                            virtual public RandomAccessFile{
    public:
        HDFSReadableFile(hdfsFS fs, std::string fileaddr) : 
            fs_(fs), fileaddr_(fileaddr), offset_(0) {
            hfile_ = hdfsOpenFile(fs, fileaddr.data(), O_RDONLY, 0, 0, 0);
        }
    private:
        hdfsFS fs_;
        std::string fileaddr_;
        hdfsFile hfile_;
        uint64_t offset_;
    public:
        virtual Status Read(size_t n, Slice* result, char* scratch) override {
            Status s;
            size_t remain = n;
            char* pos = scratch;
            while (remain > 0) {
                size_t r = hdfsRead(fs_, hfile_, pos, remain);
                if (r < 0) {
                    debug_log
                    return IOError(fileaddr_, errno);
                } else if (r == 0) {
                    break;
                }
                pos += r;
                remain -= r;
            }
            *result = Slice(scratch, n - remain);
            return Status::OK();
        }
        virtual Status Read(uint64_t offset, size_t n, Slice* result,
                            char* scratch) const override {
            size_t r = hdfsPread(fs_, hfile_, offset, scratch, n);
            if (r < 0) {
                debug_log
                return IOError(fileaddr_, errno);
            }
            *result = Slice(scratch, r);
            return Status::OK();
        }
        virtual Status Skip(uint64_t n) override {
            if (hdfsSeek(fs_, hfile_, n + offset_)) {
                debug_log
                return IOError(fileaddr_, errno);
            }
            return Status::OK();
        }
        bool isAvailable() const{
            return hfile_ != nullptr;
        }
    };
    class HDFSWritableFile : public WritableFile {
    private:
        std::string fileaddr_;
        hdfsFS fs_;
        hdfsFile hfile_;
    public:
        explicit HDFSWritableFile(hdfsFS fs, std::string fileaddr, bool appendMode = false):
            fs_(fs), fileaddr_(fileaddr) {
            printf("touch %s\n", fileaddr.data());
            if (appendMode) {
                hfile_ = hdfsOpenFile(fs, fileaddr.data(), O_WRONLY | O_APPEND, 0, 0, 0);
            } else {
                hfile_ = hdfsOpenFile(fs, fileaddr.data(), O_WRONLY, 0, 0, 0);
            }
        }
        virtual ~HDFSWritableFile() {
            printf("release %s\n", fileaddr_.data());
            if (hfile_ != nullptr) {
                Close();
            }
        }
        virtual Status Append(const Slice& data) override {
            printf("append %s\n", fileaddr_.data());
            assert(hfile_ != nullptr);
            size_t w = hdfsWrite(fs_, hfile_, reinterpret_cast<const void*>(data.data()), data.size());
            if (w < data.size()) {
                debug_log
                return IOError(fileaddr_, errno);
            }
            return Status::OK();
        }
        virtual Status Close() override {
            printf("close %s\n", fileaddr_.data());
            assert(hfile_ != nullptr);
            if (hdfsCloseFile(fs_, hfile_) != 0) {
                debug_log
                return IOError(fileaddr_, errno);
            }
            hfile_ = nullptr;
            return Status::OK();
        }
        virtual Status Flush() override {
            assert(hfile_ != nullptr);
            if (hdfsFlush(fs_, hfile_) != 0) {
                debug_log
                return IOError(fileaddr_, errno);
            }
            return Status::OK();
        }
        virtual Status Sync() override {
            assert(hfile_ != nullptr);
            Status ret = Flush();
            if (!ret.ok()) {
                return ret;
            }
            int s = hdfsHSync(fs_, hfile_);
            return ret;
        }
        bool isAvailable() const {
            assert(hfile_ != nullptr);
            return hfile_ != nullptr;
        }
    };
    class HDFSLogger : public Logger {
    private:
        hdfsFS fs_;
        HDFSWritableFile* file_;
        uint64_t (*gettid_) ();
    public:
        HDFSLogger(hdfsFS fs, HDFSWritableFile* file, uint64_t (*gettid)()):
            fs_(fs), file_(file), gettid_(gettid) { }
        //source from leveldb. edit to fit hdfs file.
        virtual void Logv(const char* format, va_list ap) override {
            const uint64_t thread_id = (*gettid_)();

            // We try twice: the first time with a fixed-size stack allocated buffer,
            // and the second time with a much larger dynamically allocated buffer.
            char buffer[500];
            for (int iter = 0; iter < 2; iter++) {
                char* base;
                int bufsize;
                if (iter == 0) {
                    bufsize = sizeof(buffer);
                    base = buffer;
                } else {
                    bufsize = 30000;
                    base = new char[bufsize];
                }
                char* p = base;
                char* limit = base + bufsize;

                struct timeval now_tv;
                gettimeofday(&now_tv, NULL);
                const time_t seconds = now_tv.tv_sec;
                struct tm t;
                localtime_r(&seconds, &t);
                p += snprintf(p, limit - p,
                              "%04d/%02d/%02d-%02d:%02d:%02d.%06d %llx ",
                              t.tm_year + 1900,
                              t.tm_mon + 1,
                              t.tm_mday,
                              t.tm_hour,
                              t.tm_min,
                              t.tm_sec,
                              static_cast<int>(now_tv.tv_usec),
                              static_cast<long long unsigned int>(thread_id));

                // Print the message
                if (p < limit) {
                    va_list backup_ap;
                    va_copy(backup_ap, ap);
                    p += vsnprintf(p, limit - p, format, backup_ap);
                    va_end(backup_ap);
                }

                // Truncate to available space if necessary
                if (p >= limit) {
                    if (iter == 0) {
                        continue;       // Try again with larger buffer
                    } else {
                        p = limit - 1;
                    }
                }

                // Add newline if necessary
                if (p == base || p[-1] != '\n') {
                    *p++ = '\n';
                }

                assert(p <= limit);
                file_->Append(Slice(base, p - base));
                file_->Flush();
                if (base != buffer) {
                    delete[] base;
                }
                break;
            }

        }
    };
    
    Status EnvHDFS::NewSequentialFile(const std::string& fname,
                                      SequentialFile** result) {
        HDFSReadableFile* rf = new HDFSReadableFile(fs_, fname);
        if (rf == nullptr || !rf->isAvailable()) {
            delete rf;
            *result = nullptr;
            debug_log
            return IOError(fname, errno);
        }
        *result = dynamic_cast<SequentialFile*>(rf);
        return Status::OK();
    }

    Status EnvHDFS::NewRandomAccessFile(const std::string& fname,
                                       RandomAccessFile** result) {
        HDFSReadableFile* file = new HDFSReadableFile(fs_, fname);
        if (file == nullptr || !file->isAvailable()) {
            delete file;
            *result = nullptr;
            debug_log
            return IOError(fname, errno);
        }
        *result = dynamic_cast<RandomAccessFile*>(file);
        return Status::OK();
    }

    Status EnvHDFS::NewWritableFile(const std::string& fname,
                                    WritableFile** result) {
        HDFSWritableFile* file = new HDFSWritableFile(fs_, fname);
        if (file == nullptr || !file->isAvailable()) {
            delete file;
            *result = nullptr;
            debug_log
            return IOError(fname, errno);
        }
        *result = dynamic_cast<WritableFile*>(file);
        return Status::OK();
    }

    Status EnvHDFS::NewAppendableFile(const std::string& fname,
                                      WritableFile** result) {
        HDFSWritableFile* file = new HDFSWritableFile(fs_, fname, true);
        if (file == nullptr || file->isAvailable()) {
            delete file;
            *result = nullptr;
            debug_log
            return IOError(fname, errno);
        }
        *result = dynamic_cast<WritableFile*>(file);
        return Status::OK();
    }
    bool EnvHDFS::FileExists(const std::string& fname) {
        return hdfsExists(fs_, fname.data()) == 0;
    }
    Status EnvHDFS::GetChildren(const std::string& dir,
                               std::vector<std::string>* result) {
        if (!FileExists(dir)) {
            debug_log
            return IOError(dir, errno);
        }
        int count = 0;
        hdfsFileInfo* allinfo = hdfsListDirectory(fs_, dir.data(), &count);
        if (allinfo == nullptr || count < 0) {
            debug_log
            return IOError(dir, errno);
        }
        for (int i = 0; i < count; ++i) {
            char* tmp = rindex(allinfo[i].mName, '/');
            if (tmp != nullptr) {
                result->emplace_back(std::string(tmp));
            }
        }
        hdfsFreeFileInfo(allinfo, count);
        return Status::OK();
    }
    Status EnvHDFS::DeleteFile(const std::string& fname) {
        if (hdfsDelete(fs_, fname.data(), 0)) {
            debug_log
            return IOError(fname, errno);
        }
        return Status::OK();
    }
    Status EnvHDFS::CreateDir(const std::string& dirname) {
        if (hdfsCreateDirectory(fs_, dirname.data())) {
            debug_log
            return IOError(dirname, errno);
        }
        return Status::OK();
    }
    Status EnvHDFS::DeleteDir(const std::string& dirname) {
        if (hdfsDelete(fs_, dirname.data(), 1)) {
            debug_log
            return IOError(dirname, errno);
        }
        return Status::OK();
    }
    Status EnvHDFS::GetFileSize(const std::string& fname, uint64_t* file_size) {
        hdfsFileInfo* info = hdfsGetPathInfo(fs_, fname.data());
        if (info != nullptr) {
            *file_size = info->mSize;
            hdfsFreeFileInfo(info, 1);
            return Status::OK();
        }
        debug_log
        return IOError(fname, errno);
    }
    Status EnvHDFS::RenameFile(const std::string& src,
                              const std::string& target) {
        DeleteFile(target);
        if (hdfsRename(fs_, src.data(), target.data())) {
            debug_log
            return IOError(src, errno);
        }
        return Status::OK();
    }
    Status EnvHDFS::LockFile(const std::string& fname, FileLock** lock) {
        //To avoid conflict to mirror mode.
        return Status::OK();
    }
    Status EnvHDFS::UnlockFile(FileLock* lock) {
        //To avoid conflict to mirror mode.
        return Status::OK();
    }
    Status EnvHDFS::NewLogger(const std::string& fname, Logger** result) {
        HDFSWritableFile* file = new HDFSWritableFile(fs_, fname);
        if (file == nullptr || !file->isAvailable()) {
            delete file;
            *result = nullptr;
            debug_log
            return IOError(fname, errno);
        }
        HDFSLogger* logger = new HDFSLogger(fs_, file, &EnvHDFS::gettid);
        if (logger == nullptr) {
            delete file;
            delete logger;
            debug_log
            return IOError(fname, errno);
        }
        *result = dynamic_cast<Logger*>(logger);
        return Status::OK();
    }
    EnvHDFS* NewEnvHDFS(const std::string& fname) {
        EnvHDFS* ret = new EnvHDFS(fname);
        return ret;
    }
}

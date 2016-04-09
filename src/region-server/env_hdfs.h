#ifndef BOLERO_ENV_HDFS_H
#define BOLERO_ENV_HDFS_H

#include "leveldb/env.h"
#include "hdfs.h"
namespace bolero {
    using namespace ::leveldb;
    class EnvHDFS : public Env {
    public:
        explicit EnvHDFS(const std::string& fsname) : fsname_(fsname) {
            dftEnv_ = Env::Default();
            char* host = new char[fsname.length() + 1];
            host[0] = 0;
            int port = -1;
            int s = sscanf(fsname.data(), "hdfs://%[^:]:%d", host, &port);
            if (s < 2) {
                delete host;
                fs_ = nullptr;
                return;
            }
            assert(host[0] != 0);
            assert(port > 0);
            fs_ = hdfsConnect(host, port);
            delete host;
        }

        virtual ~EnvHDFS() {
            fprintf(stderr, "Destroying HdfsEnv::Default()\n");
            hdfsDisconnect(fs_);
        }

        virtual Status NewSequentialFile(const std::string& fname,
                                         SequentialFile** result) override;

        virtual Status NewRandomAccessFile(const std::string& fname,
                                           RandomAccessFile** result) override;

        virtual Status NewWritableFile(const std::string& fname,
                                       WritableFile** result) override;

        virtual Status NewAppendableFile(const std::string& fname,
                                         WritableFile** result) override;

        virtual bool FileExists(const std::string& fname) override;

        virtual Status GetChildren(const std::string& dir,
                                   std::vector<std::string>* result) override;

        virtual Status DeleteFile(const std::string& fname) override;

        virtual Status CreateDir(const std::string& dirname) override;

        virtual Status DeleteDir(const std::string& dirname) override;

        virtual Status GetFileSize(const std::string& fname, uint64_t* file_size) override;

        virtual Status RenameFile(const std::string& src,
                                  const std::string& target) override;

        virtual Status LockFile(const std::string& fname, FileLock** lock) override;

        virtual Status UnlockFile(FileLock* lock) override;
        virtual void Schedule(void (*function)(void* arg), void* arg) override {
            dftEnv_->Schedule(function, arg);
        }
        virtual void StartThread(void (*function)(void* arg), void* arg) override {
            dftEnv_->StartThread(function, arg);
        }
        virtual Status GetTestDirectory(std::string* path) override {
            return dftEnv_->GetTestDirectory(path);
        }
        virtual Status NewLogger(const std::string& fname, Logger** result) override;
        virtual uint64_t NowMicros() override {
            return dftEnv_->NowMicros();
        }
        virtual void SleepForMicroseconds(int micros) override {
            return dftEnv_->SleepForMicroseconds(micros);
        }
        static uint64_t gettid() {
            return static_cast<uint64_t>(pthread_self());
        }
    private:
        hdfsFS fs_;
        std::string fsname_;
        Env* dftEnv_;
    };
    extern EnvHDFS* NewEnvHDFS(const std::string& fsname);
}//namespace bolero

#endif

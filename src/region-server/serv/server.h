#ifndef BOLERO_SERVER_H
#define BOLERO_SERVER_H

#include "util/region_config.h"
#include "leveldb/db.h"

#include <vector>
#include <string>

namespace bolero {
    class Server {
    public:
    Server(): db(nullptr), cur_env(nullptr),config_() { }
        bool init(const std::string& config_file);
        leveldb::ReadOptions default_read_options();
        leveldb::WriteOptions default_write_options();

        leveldb::Status hget(leveldb::Slice& user_key, leveldb::Slice& field, std::string* value);
        leveldb::Status hmget(leveldb::Slice& user_key, const std::vector<leveldb::Slice> fields,
                              std::vector<std::string>* value);

        leveldb::Status hset(leveldb::Slice& user_key, leveldb::Slice& field, leveldb::Slice& value);
        leveldb::Status hmset(leveldb::Slice& user_key, const std::vector<std::pair<leveldb::Slice, leveldb::Slice>> kvs);

        leveldb::DB* db;
        leveldb::Env* cur_env;
    private:
        RegionConfig config_;
    };
}//namespace bolero

#endif

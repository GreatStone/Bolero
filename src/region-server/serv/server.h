#ifndef BOLERO_SERVER_H
#define BOLERO_SERVER_H

#include "util/region_config.h"
#include "util/env_hdfs.h"
#include "leveldb/db.h"

#include <vector>
#include <string>

namespace bolero {
    class Server {
    public:
        Server(): db(nullptr), cur_env(nullptr),config_() { }
        ~Server() {
            delete db;
            delete cur_env;
        }
        bool init(const std::string& config_file);
        leveldb::ReadOptions default_read_options() const {return config_.default_ropt();}
        leveldb::WriteOptions default_write_options() const {return config_.default_wopt();}

        leveldb::Status hget(leveldb::Slice user_key, leveldb::Slice field, std::string* value);
        leveldb::Status hmget(leveldb::Slice user_key, const std::vector<leveldb::Slice>& fields,
                              std::vector<std::string>* value);

        leveldb::Status hset(leveldb::Slice user_key, leveldb::Slice field, leveldb::Slice value);
        leveldb::Status hmset(leveldb::Slice user_key, const std::vector<std::pair<leveldb::Slice, leveldb::Slice>>& kvs);

        leveldb::Status hdel(leveldb::Slice user_key, leveldb::Slice field);
        leveldb::Status hmdel(leveldb::Slice user_key, const std::vector<leveldb::Slice>& fields);
    private:
        leveldb::DB* db;
        leveldb::Env* cur_env;
    public:
        RegionConfig config_;
    };
}//namespace bolero

#endif

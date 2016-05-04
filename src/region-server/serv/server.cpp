#include "server.h"

#include "util/env_hdfs.h"
#include "leveldb/db.h"
#include "format.h"

#include <string>
#include <vector>

namespace bolero {
    bool Server::init(const std::string& config_file) {
        if (!config_.read_from_file(config_file)) {
            return false;
        }

        cur_env = NewEnvHDFS(config_.fs_addr);
        if (cur_env == nullptr) {return false;}

        //TODO: init zookeeper.

        leveldb::Options db_opt = config_.default_dbopt();
        db_opt.env = cur_env;
        leveldb::Status status = leveldb::DB::Open(db_opt, config_.db_location, &db);
        if (!status.ok()) {
            perror(status.ToString().data());
            return false;
        }
        return true;
    }
    leveldb::Status Server::hget(leveldb::Slice user_key, leveldb::Slice field, std::string* value) {
        std::string hash_key;
        encode_hash_key(user_key, field, &hash_key);
        return db->Get(default_read_options(), hash_key, value);
    }
    leveldb::Status Server::hmget(leveldb::Slice user_key, const std::vector<Slice>& fields,
                          std::vector<std::string>* value) {
        std::string cur;
        leveldb::Status s;
        for (Slice field: fields) {
            s = hget(user_key, field, &cur);
            if (!s.ok()) {
                if (s.IsNotFound()) {
                    value->emplace_back(std::string(DataType::DATA_NULL, 1));
                    continue;
                }
                return s;
            }
            value->emplace_back(std::string(DataType::DATA_HASH, 1) + cur);
        }
        return leveldb::Status::OK();
    }
    leveldb::Status Server::hset(leveldb::Slice user_key, leveldb::Slice field, leveldb::Slice value) {
        std::string hash_key;
        encode_hash_key(user_key, field, &hash_key);
        return db->Put(default_write_options(), hash_key, value);
    }
    leveldb::Status Server::hmset(leveldb::Slice user_key,
                          const std::vector<std::pair<leveldb::Slice, leveldb::Slice>>& kvs) {
        std::string cur;
        leveldb::Status s;
        for (auto kv: kvs) {
            s = hset(user_key, kv.first, kv.second);
            if (!s.ok()) {
                return s;
            }
        }
        return leveldb::Status::OK();
    }
    leveldb::Status Server::hdel(leveldb::Slice user_key, leveldb::Slice field) {
        std::string hash_key;
        encode_hash_key(user_key, field, &hash_key);
        return db->Delete(default_write_options(), hash_key);
    }
    leveldb::Status Server::hmdel(leveldb::Slice user_key, const std::vector<leveldb::Slice>& fields) {
        std::string cur;
        leveldb::Status s;
        for (auto field: fields) {
            s = hdel(user_key, field);
            if (!s.ok()) {
                return s;
            }
        }
        return leveldb::Status::OK();
    }
}//namespace bolero

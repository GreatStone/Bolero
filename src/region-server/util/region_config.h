#ifndef BOLERO_REGION_CONFIG_H
#define BOLERO_REGION_CONFIG_H

#include "leveldb/options.h"

#include <string>
#include <stdint.h>

namespace bolero {
    class RegionConfig {
    public:
        RegionConfig(): zk_host(), region_ppath(), region_name(), fs_addr(),
                        remote_itercache_size(1024), db_ropt(), db_wopt(){}
        std::string zk_host;
        std::string region_ppath;
        std::string region_name;
        std::string fs_addr;
        uint32_t remote_itercache_size;
        std::string db_location;
        bool read_from_file(const std::string& file_path);
        leveldb::ReadOptions default_ropt() const {
            return db_ropt;
        }
        leveldb::WriteOptions default_wopt() const {
            return db_wopt;
        }
        leveldb::Options default_dbopt() const {
            return opt;
        }
    private:
        struct leveldb::ReadOptions db_ropt;
        struct leveldb::WriteOptions db_wopt;
        struct leveldb::Options opt;
    };
}

#endif //BOLERO_REGION_CONFIG_H

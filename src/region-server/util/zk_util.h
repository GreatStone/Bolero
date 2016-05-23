#ifndef BOLERO_ZK_UTIL_H
#define BOLERO_ZK_UTIL_H

#include "zookeeper/zookeeper.h"

#include <pthread.h>

#include <string>

namespace bolero {
    class RegionConfig;
    extern bool register_server(const RegionConfig& config);
    struct ZkRegionServer {
        zhandle_t* handle;
        std::string region_name;
        std::string serv_addr;
        std::string fs_addr;
        int readable:1;
        int writable:1;
        pthread_mutex_t mutex;
    };
}//namespace bolero

#endif //BOLERO_ZK_UTIL_H

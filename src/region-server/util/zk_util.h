#ifndef BOLERO_ZK_UTIL_H
#define BOLERO_ZK_UTIL_H

#include "zookeeper/zookeeper.h"

#include <string>

struct ZkRegionServer {
    std::string region_name = "";
    int readable:1 = 0;
    int writable:1 = 0;
};

template <int HashSize>
class HashUtil {
 public:
    
 private:
    ZkRegionServer* servers[];
}

#endif //BOLERO_ZK_UTIL_H

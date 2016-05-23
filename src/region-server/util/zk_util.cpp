#include "zk_util.h"

#include "region_config.h"
#include "locking.h"

#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace bolero{
#ifndef BOLERO_ZK_TIMEOUT_TICK
    static const int zk_timeout_tick = 30000;
#else
    static const int zk_timeout_tick = BOLERO_ZK_TIMEOUT_TICK;
#endif
    static void zk_callback(zhandle_t *zh, int type, int state,
                            const char *path, void *watcherCtx) {
    }
    static const char* get_localip() {
        static const char* LOCALHOSTIP = "127.0.0.1";
        struct ifaddrs * ifAddrStruct = nullptr;
        struct ifaddrs * ifa = nullptr;
        void * tmpAddrPtr = nullptr;
        static char addressBuffer[INET6_ADDRSTRLEN + 5];
        addressBuffer[0] = '\0';

        getifaddrs(&ifAddrStruct);
        for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
            if (ifa ->ifa_addr->sa_family == AF_INET) {
                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                if (strcmp(addressBuffer, LOCALHOSTIP) == 0) {
                    continue;
                }
                break;
            } else if (ifa->ifa_addr->sa_family == AF_INET6) {
                continue;
            }
        }
        if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);
        return addressBuffer;
    }
    static struct ZkRegionServer* local_region(const RegionConfig* config) {
        static ZkRegionServer* region = nullptr;
        if (region == nullptr) {
            if (config == nullptr) {
                return nullptr;
            }
            region = new struct ZkRegionServer;
            region->region_name = config->region_name;

            std::string addr(get_localip());
            static char tmp[20];
            sprintf(tmp, "%u", config->region_port);
            region->serv_addr = addr + ":" + tmp;
            region->fs_addr = region->fs_addr;
            region->handle = zookeeper_init(config->zk_host.data(), zk_callback,
                                            zk_timeout_tick, 0, nullptr, 0);
            region->readable = 1;
            region->writable = 1;
            pthread_mutex_init(&region->mutex, nullptr);
        }
        return region;
    }
    bool register_server(const RegionConfig& config) {
        struct ZkRegionServer* cur = local_region(&config);
        MutexLocking l(&(cur->mutex));
        std::string region_path = config.region_ppath + '/' + config.region_name;
        struct Stat stat;
        if (!zoo_exists(cur->handle, config.region_ppath.data(), 0, &stat)) {
            int ret = zoo_create(cur->handle, region_path.data(), config.fs_addr.data(),
                                 config.fs_addr.size(), &ZOO_OPEN_ACL_UNSAFE, 0,
                                 nullptr, -1);
            if (ret != ZOK) {
                return false;
            }
        }
        std::string server_path = region_path + '/' + config.region_name;
        int ret = zoo_create(cur->handle, server_path.data(), cur->serv_addr.data(),
                             cur->serv_addr.size(), &ZOO_OPEN_ACL_UNSAFE,
                             ZOO_EPHEMERAL | ZOO_SEQUENCE, nullptr, -1);
        return (ret == ZOK);
    }
}//namespace bolero

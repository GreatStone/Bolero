#include "rpc/proxy_config.h"
#include "rpc/rpc_center.h"
#include "sofa/pbrpc/pbrpc.h"
#include "leveldb/db.h"

#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s [config-file]\n", argv[0]);
        return -1;
    }
    bolero::ProxyConfig config;
    if (!config.init(argv[1])) {
        printf("Fail to load config %s\n", argv[1]);
        return -1;
    }
    bolero::RpcCenter center("gscluster.node1:12321");
    center.set_config(&config);
    if (!center.init()) {
        printf("Fail to init center of RPC.\n");
        return -1;
    }
    for (int i = 0; i < 100; ++i) {
        std::string key = "test";
        key.append(reinterpret_cast<char*>(&i), sizeof(int));
        int e = i * i;
        center.hset(key, "a", leveldb::Slice(reinterpret_cast<char*>(&e), sizeof(int)));
    }
    for (int i = 0; i < 100; ++i) {
        std::string key = "test";
        key.append(reinterpret_cast<char*>(&i), sizeof(int));
        std::string e;
        center.hget(key, "a", &e);
        printf("get %d\n", *reinterpret_cast<const int*>(e.data()));
    }
    return 0;
}

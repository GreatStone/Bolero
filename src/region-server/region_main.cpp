#include <cstdio>
#include <assert.h>
#include <leveldb/db.h>
#include <vector>
#include <string>

#include "serv/server.h"
#include "serv/region_server.h"
#include "util/zk_util.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s [config-file]\n", argv[0]);
        return -1;
    }
    SOFA_PBRPC_SET_LOG_LEVEL(INFO);
    bolero::Server server;
    if (!server.init(argv[1])) {
        SLOG(FATAL, "Fail to init local server");
        return -1;
    }
    bolero::RegionServer region;
    region.set_local_server(&server);
    if (!region.init()) {
        SLOG(FATAL, "Fail to init region server");
        return -1;
    }
    if (!register_server(server.config_)) {
        SLOG(FATAL, "Fail to register on zookeeper: %s", server.config_.zk_host.data());
        return -1;
    }
    region.server_run();
    return 0;
}

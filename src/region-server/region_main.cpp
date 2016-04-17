#include <cstdio>
#include <assert.h>
#include <leveldb/db.h>
#include <vector>
#include <string>

#include "serv/server.h"
#include "serv/region_server.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s [config-file]\n", argv[0]);
        return -1;
    }
    bolero::Server server;
    if (!server.init(argv[1])) {
        printf("Fail to init local server\n");
        return -1;
    }
    bolero::RegionServer region;
    region.set_local_server(&server);
    if (!region.init()) {
        printf("Fail to init region server\n");
        return -1;
    }
    region.server_run();
    
    return 0;
}

#include "rpc/proxy_config.h"
#include "rpc/rpc_center.h"
#include "forward/forward_center.h"

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
    SOFA_PBRPC_SET_LOG_LEVEL(NOTICE);
    bolero::ForwardCenter forward(&config);
    forward.start_listen();
    SLOG(INFO, "started.\n");
    for(;;);
    return 0;
}

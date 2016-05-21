#include "rpc/proxy_config.h"
#include "rpc/rpc_center.h"
#include "forward/forward_center.h"

#include "sofa/pbrpc/pbrpc.h"
#include "leveldb/db.h"

#include <string>

using namespace bolero;

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
    shared_ptr<RpcCenter> rpc(new RpcCenter);
    rpc->set_config(&config);

    shared_ptr<ForwardCenter> forward(new ForwardCenter(&config));
    rpc->set_forward_center(forward);
    forward->set_rpc_center(rpc);
    SOFA_PBRPC_SET_LOG_LEVEL(DEBUG);
    if (!rpc->init()) {
        SLOG(INFO, "rpc init failed");
        printf("rpc init failed\n");
        return -1;
    }
    forward->start_listen();
    SLOG(INFO, "started.");
    for(;;);
    return 0;
}

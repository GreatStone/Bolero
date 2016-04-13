#ifndef BOLERO_PROXY_CONFIG_H
#define BOLERO_PROXY_CONFIG_H

#include <string>
#include <stdint.h>

#include "sofa/pbrpc/pbrpc.h"

namespace bolero {
    class ProxyConfig {
    public:
        ProxyConfig(): zk_host(), zk_region_ppath(), client_opts_() { }
        bool init(const std::string& file_name);
        std::string zk_host;
        std::string zk_region_ppath;
        struct ::sofa::pbrpc::RpcClientOptions default_client_opt() {
            return client_opts_;
        }
    private:
        struct ::sofa::pbrpc::RpcClientOptions client_opts_;
    };
}//namespace bolero

#endif //BOLERO_REGION_CONFIG_H

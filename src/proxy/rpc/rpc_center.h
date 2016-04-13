#ifndef BOLERO_RPC_CENTER_H
#define BOLERO_RPC_CENTER_H

#include "proxy_config.h"

#include "leveldb/db.h"
#include "sofa/pbrpc/pbrpc.h"
#include "proto/region_server_proto.pb.h"

namespace bolero {
    const int kMaxRetry = 3;
    class RpcCenter {
    public:
        RpcCenter(const std::string & server_host): 
            server_host_(server_host),
            client_(nullptr), rpc_channel_(nullptr),
            stub_(nullptr) { }
        ~RpcCenter() {
            delete client_;
            delete stub_;
        }
        bool init ();
        static void set_config(ProxyConfig* config) {
            config_ = config;
        }
        static ProxyConfig default_config() {
            return *config_;
        }
        bool hget(leveldb::Slice user_key, leveldb::Slice field,
                  std::string* value, ::google::protobuf::Closure* done = nullptr,
                  int64_t time_out = 10000);
        bool hset(leveldb::Slice user_key, leveldb::Slice field,
                  leveldb::Slice value, ::google::protobuf::Closure* done = nullptr,
                  int64_t time_out = 10000);
    private:
        static ProxyConfig* config_;
        std::string server_host_;
        ::sofa::pbrpc::RpcClient* client_;
        ::sofa::pbrpc::RpcChannel* rpc_channel_;
        ::bolero::proto::RegionServerProto_Stub* stub_;
    };
}//namespace bolero

#endif

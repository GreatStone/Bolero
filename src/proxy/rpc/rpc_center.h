#ifndef BOLERO_RPC_CENTER_H
#define BOLERO_RPC_CENTER_H

#include "proxy_config.h"

#include "forward/forward_center.h"

#include "leveldb/db.h"
#include "sofa/pbrpc/pbrpc.h"
#include "proto/region_server_proto.pb.h"

#include <memory>

using std::shared_ptr;

namespace bolero {
    const int kMaxRetry = 3;
    typedef void (* proxy_rpc_callback_t) (::bolero::proto::HashRequest* request,
                                          ::bolero::proto::HashResponse* response,
                                          ::sofa::pbrpc::RpcController* ctnl);
    class RpcCenter {
    public:
        RpcCenter(): 
            client_(nullptr), rpc_channels_(),
            rpc_stubs_() { }
        ~RpcCenter() {
            delete client_;
        }
        bool init ();
        static void set_config(ProxyConfig* config) {
            config_ = config;
        }
        static ProxyConfig default_config() {
            return *config_;
        }
        void set_forward_center(shared_ptr<ForwardCenter> forward_center) {
            forward_center_ = forward_center;
        }
        bool hget(leveldb::Slice user_key, leveldb::Slice field,
                  ::google::protobuf::Closure* done = nullptr,
                  int64_t time_out = 10000);
        bool hset(leveldb::Slice user_key, leveldb::Slice field,
                  leveldb::Slice value, ::google::protobuf::Closure* done = nullptr,
                  int64_t time_out = 10000);
        bool hdel(leveldb::Slice user_key, leveldb::Slice field,
                  ::google::protobuf::Closure* done = nullptr,
                  int64_t time_out = 10000);
        bool hmget(leveldb::Slice user_key, const std::vector<leveldb::Slice>& fields,
                   ::google::protobuf::Closure* done = nullptr,
                   int64_t time_out = 10000);
        bool hmset(leveldb::Slice user_key,
                   const std::vector<std::pair<leveldb::Slice, leveldb::Slice>>& kvs,
                   ::google::protobuf::Closure* done = nullptr,
                   int64_t time_out = 10000);
        bool hmdel(leveldb::Slice user_key, const std::vector<leveldb::Slice>& field,
                  ::google::protobuf::Closure* done = nullptr,
                  int64_t time_out = 10000);
    private:
        shared_ptr<::bolero::proto::RegionServerProto_Stub> get_stub(const std::string& addr);
        static ProxyConfig* config_;
        ::sofa::pbrpc::RpcClient* client_;
        std::map<std::string, shared_ptr<::sofa::pbrpc::RpcChannel>> rpc_channels_;
        std::map<std::string, shared_ptr<::bolero::proto::RegionServerProto_Stub>> rpc_stubs_;
        shared_ptr<ForwardCenter> forward_center_;
    };
}//namespace bolero

#endif

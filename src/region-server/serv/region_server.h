#include "sofa/pbrpc/pbrpc.h"
#include "proto/region_server_proto.pb.h"
#include "server.h"

namespace bolero {
    class RegionServer : public ::bolero::proto::RegionServerProto {
    public:
        RegionServer(): 
            local_server_(nullptr), rpc_server_(nullptr), options_() {}
        virtual ~RegionServer() {
            delete rpc_server_;
            delete local_server_;
        }
        bool init();
        virtual void hash_op(google::protobuf::RpcController* controller,
                             const ::bolero::proto::HashRequest* request,
                             ::bolero::proto::HashResponse* response,
                             google::protobuf::Closure* done) override;
        void set_local_server(Server* server) {local_server_ = server;}
        void set_rpc_options(sofa::pbrpc::RpcServerOptions options) {options_ = options;}
        void server_run() {
            rpc_server_->Run();
        }
        void handle_hget(const ::bolero::proto::HashRequest* request,
                         ::bolero::proto::HashResponse* response);
        void handle_hset(const ::bolero::proto::HashRequest* request,
                         ::bolero::proto::HashResponse* response);
        void handle_hdel(const ::bolero::proto::HashRequest* request,
                         ::bolero::proto::HashResponse* response);
        void handle_hmget(const ::bolero::proto::HashRequest* request,
                          ::bolero::proto::HashResponse* response);
        void handle_hmset(const ::bolero::proto::HashRequest* request,
                          ::bolero::proto::HashResponse* response);
        void handle_hmdel(const ::bolero::proto::HashRequest* request,
                          ::bolero::proto::HashResponse* response);
    private:
        Server* local_server_;
        sofa::pbrpc::RpcServer* rpc_server_;
        sofa::pbrpc::RpcServerOptions options_;
    };
}//namespace bolero

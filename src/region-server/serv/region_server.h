#include "sofa/pbrpc/pbrpc.h"
#include "proto/region_server_proto.pb.h"
#include "server.h"

namespace bolero {
    class RegionServer : public ::bolero::proto::RegionServerProto {
    public:
        RegionServer(): 
            local_server_(nullptr) {}
            virtual ~RegionServer() {}
        bool init();
        virtual void hash_op(google::protobuf::RpcController* controller,
                             const ::bolero::proto::HashRequest* request,
                             ::bolero::proto::HashResponse* response,
                             google::protobuf::Closure* done) override;
        void set_local_server(Server* server) {local_server_ = server;}
    private:
        Server* local_server_;
    };
}

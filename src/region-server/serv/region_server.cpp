#include "region_server.h"

#include "sofa/pbrpc/pbrpc.h"

namespace bolero {
    bool RegionServer::init() {
        if (local_server_ == nullptr) {
            return false;
        }
        SOFA_PBRPC_SET_LOG_LEVEL(NOTICE);

        //TODO configurable.

        sofa::pbrpc::RpcServerOptions options;
        options.work_thread_num = 8;
        sofa::pbrpc::RpcServer rpc_server(options);

        if (!rpc_server.Start("0.0.0.0:12321")) {
            SLOG(ERROR, "start server failed");
            return false;
        }

        ::bolero::proto::RegionServerProto* region_service = dynamic_cast<::bolero::proto::RegionServerProto*>(new RegionServer());
        if (!rpc_server.RegisterService(region_service)) {
            SLOG(ERROR, "register service failed");
            return false;
        }
        return true;
    }
    typedef std::pair<const ::bolero::proto::HashRequest*, std::pair<::bolero::proto::HashResponse*, ::google::protobuf::Closure*> > PackedArgs;
    static void* hash_handle(void* args) {
        PackedArgs* rargs = reinterpret_cast<PackedArgs*>(args);
        const ::bolero::proto::HashRequest* request = rargs->first;
        ::bolero::proto::HashResponse* response = rargs->second.first;
        google::protobuf::Closure* done = rargs->second.second;
        assert(request != nullptr && response != nullptr && done != nullptr);
        switch (request->operation()) {
        case ::bolero::proto::HashResponse::HGET:
            SLOG(FATAL, "HGET");
        case ::bolero::proto::HashResponse::HSET:
            SLOG(FATAL, "HSET");
        case ::bolero::proto::HashResponse::HMSET:
            SLOG(FATAL, "HMSET");
        case ::bolero::proto::HashResponse::HMGET:
            SLOG(FATAL, "HMGET");
        }
        done->Run();
        delete rargs;
        return nullptr;
    }

    void RegionServer::hash_op(google::protobuf::RpcController* controller,
                               const ::bolero::proto::HashRequest* request,
                               ::bolero::proto::HashResponse* response,
                               google::protobuf::Closure* done) {
        sofa::pbrpc::RpcController* cntl =
            static_cast<sofa::pbrpc::RpcController*>(controller);
        SLOG(NOTICE, "HashOP(): request message from %s",
             cntl->RemoteAddress().c_str());
        pthread_t pid;
        PackedArgs* arg = new PackedArgs;
        arg->first = request;
        arg->second.first = response;
        arg->second.second = done;
        if (pthread_create(&pid, NULL, hash_handle, arg) != 0) {
            delete arg;
            SLOG(FATAL, "Fail to create thread. From %s", cntl->RemoteAddress().data());
            done->Run();
        }
    }
}//namespace bolero

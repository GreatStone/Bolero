#include "region_server.h"

#include "sofa/pbrpc/pbrpc.h"

namespace bolero {
    bool RegionServer::init() {
        if (local_server_ == nullptr) {
            return false;
        }
        SOFA_PBRPC_SET_LOG_LEVEL(NOTICE);

        rpc_server_ = new sofa::pbrpc::RpcServer(options_);

        std::string host = "0.0.0.0:";
        char tmp[20];
        sprintf(tmp, "%d", local_server_->config_.region_port);
        host.append(tmp);

        if (!rpc_server_->Start(host.data())) {
            SLOG(ERROR, "start server failed");
            return false;
        }

        ::bolero::proto::RegionServerProto* region_service = dynamic_cast<::bolero::proto::RegionServerProto*>(this);
        if (!rpc_server_->RegisterService(region_service)) {
            SLOG(ERROR, "register service failed");
            return false;
        }
        return true;
    }
    void RegionServer::handle_hget(const ::bolero::proto::HashRequest* request,
                                   ::bolero::proto::HashResponse* response) {
        if (!request->has_user_key() || request->user_key().empty()) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return;
        } else if (!request->has_req_batch() || request->req_batch().empty()) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return;
        }
        response->set_user_key(request->user_key());
        leveldb::Slice user_key(request->user_key());
        leveldb::Slice field(request->req_batch());
        std::string value;
        leveldb::Status s = local_server_->hget(user_key, field, &value);
        if (s.ok()) {
            response->set_res_batch(value);
            return;
        } else if (s.IsNotFound()) {
            return;
        }
        response->set_err(::bolero::proto::HashResponse::DB_ERROR);
    }
    void RegionServer::handle_hset(const ::bolero::proto::HashRequest* request,
                                   ::bolero::proto::HashResponse* response) {
        if (!request->has_user_key() || request->user_key().empty()) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return;
        } else if (!request->has_req_batch() || request->req_batch().empty()) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return;
        }
        leveldb::Slice user_key(request->user_key());
        const char* buf = request->req_batch().data();

        if (request->req_batch().size() < sizeof(uint32_t)) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return;
        }

        uint32_t field_size = *reinterpret_cast<const uint32_t*>(buf);
        buf += sizeof(uint32_t);

        if (request->req_batch().size() < sizeof(uint32_t) * 2 + field_size) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return;
        }
        leveldb::Slice field(buf, field_size);
        buf += field_size;

        uint32_t value_size = *reinterpret_cast<const uint32_t*>(buf);
        buf += sizeof(uint32_t);
        if (request->req_batch().size() != sizeof(uint32_t) * 2 + field_size + value_size) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return;
        }

        leveldb::Slice value(buf, value_size);
        leveldb::Status s = local_server_->hset(user_key, field, value);
        if (s.ok()) {
            response->set_err(::bolero::proto::HashResponse::OK);
            return;
        }
        response->set_err(::bolero::proto::HashResponse::DB_ERROR);
    }
    typedef std::pair<RegionServer*, std::pair<const ::bolero::proto::HashRequest*, std::pair<::bolero::proto::HashResponse*, ::google::protobuf::Closure*> > > PackedArgs;
    static void* hash_handle(void* args) {
        PackedArgs* rargs = reinterpret_cast<PackedArgs*>(args);
        RegionServer* region = rargs->first;
        const ::bolero::proto::HashRequest* request = rargs->second.first;
        ::bolero::proto::HashResponse* response = rargs->second.second.first;
        google::protobuf::Closure* done = rargs->second.second.second;
        assert(request != nullptr && response != nullptr && done != nullptr);
        switch (request->operation()) {
        case ::bolero::proto::HashRequest::HGET:
            region->handle_hget(request, response);
            SLOG(INFO, "HGET");
            break;
        case ::bolero::proto::HashRequest::HSET:
            region->handle_hset(request, response);
            SLOG(INFO, "HSET");
            break;
        case ::bolero::proto::HashRequest::HMSET:
            SLOG(INFO, "HMSET");
            break;
        case ::bolero::proto::HashRequest::HMGET:
            SLOG(INFO, "HMGET");
            break;
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
        arg->first = this;
        arg->second.first = request;
        arg->second.second.first = response;
        arg->second.second.second = done;
        if (pthread_create(&pid, NULL, hash_handle, arg) != 0) {
            delete arg;
            SLOG(ERROR, "Fail to create thread. From %s", cntl->RemoteAddress().data());
            done->Run();
        }
    }
}//namespace bolero

#include "region_server.h"

#include "format.h"
#include "sofa/pbrpc/pbrpc.h"

namespace bolero {
    bool RegionServer::init() {
        if (local_server_ == nullptr) {
            return false;
        }

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
            int length = value.size();
            value = std::string(reinterpret_cast<char*>(&length), sizeof(int32_t)) + value;
            response->set_res_batch(std::move(value));
            response->set_err(::bolero::proto::HashResponse::OK);
            return;
        } else if (s.IsNotFound()) {
            int length = -1;
            value = std::string(reinterpret_cast<char*>(&length), sizeof(int32_t));
            response->set_res_batch(std::move(value));
            response->set_err(::bolero::proto::HashResponse::OK);
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
    void RegionServer::handle_hdel(const ::bolero::proto::HashRequest* request,
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
        leveldb::Status s = local_server_->hdel(user_key, field);
        if (s.ok() || s.IsNotFound()) {
            response->set_err(::bolero::proto::HashResponse::OK);
            return;
        }
        response->set_err(::bolero::proto::HashResponse::DB_ERROR);
    }
    void RegionServer::handle_hmget(const ::bolero::proto::HashRequest* request,
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
        int32_t field_length = -1;
        const char* req_buf = request->req_batch().data();
        const char* const req_buf_end = req_buf + request->req_batch().length();
        std::vector<leveldb::Slice> fields;
        std::vector<std::string> values;
        while (req_buf < req_buf_end) {
            if (req_buf + sizeof(int32_t) > req_buf_end) {
                response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
                return;
            }
            field_length = *reinterpret_cast<const int32_t*>(req_buf);
            req_buf += sizeof(int32_t);
            if (req_buf + field_length > req_buf_end) {
                response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
                return;
            }
            fields.emplace_back(::leveldb::Slice(req_buf, field_length));
            req_buf += field_length;
        }
        leveldb::Status s = local_server_->hmget(user_key, fields, &values);
        if (!s.ok()) {
            response->set_err(::bolero::proto::HashResponse::DB_ERROR);
            return;
        }
        std::string rsp_batch;
        int32_t length;
        for (std::string e : values) {
            length = e.size() - 1;
            if (e[0] == DataType::DATA_HASH){
                rsp_batch.append(reinterpret_cast<char*>(&length), sizeof(int32_t));
                rsp_batch.append(e.data() + 1, length);
            } else {
                length = -1;
                rsp_batch.append(reinterpret_cast<char*>(&length), sizeof(int32_t));
            }
        }
        response->set_res_batch(std::move(rsp_batch));
        response->set_err(::bolero::proto::HashResponse::OK);
    }
    void RegionServer::handle_hmset(const ::bolero::proto::HashRequest* request,
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
        int32_t field_length = -1;
        int32_t value_length = -1;
        const char* req_buf = request->req_batch().data();
        const char* const req_buf_end = req_buf + request->req_batch().length();
        std::vector<std::pair<leveldb::Slice, leveldb::Slice>> kvs;
        const char* field_pos;
        while (req_buf < req_buf_end) {
            if (req_buf + sizeof(int32_t) > req_buf_end) {
                response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
                return;
            }
            field_length = *reinterpret_cast<const int32_t*>(req_buf);
            req_buf += sizeof(int32_t);
            if (req_buf + field_length > req_buf_end) {
                response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
                return;
            }
            field_pos = req_buf;
            req_buf += field_length;
            if (req_buf + sizeof(int32_t) > req_buf_end) {
                response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
                return;
            }
            value_length = *reinterpret_cast<const int32_t*>(req_buf);
            req_buf += sizeof(int32_t);
            if (req_buf + value_length > req_buf_end) {
                response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
                return;
            }
            kvs.emplace_back(std::make_pair(::leveldb::Slice(field_pos, field_length), ::leveldb::Slice(req_buf, value_length)));
            req_buf += value_length;
        }
        leveldb::Status s = local_server_->hmset(user_key, kvs);
        if (!s.ok()) {
            response->set_err(::bolero::proto::HashResponse::DB_ERROR);
            return;
        }
        response->set_err(::bolero::proto::HashResponse::OK);
    }
    void RegionServer::handle_hmdel(const ::bolero::proto::HashRequest* request,
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
        int32_t field_length = -1;
        const char* req_buf = request->req_batch().data();
        const char* const req_buf_end = req_buf + request->req_batch().length();
        std::vector<leveldb::Slice> fields;
        while (req_buf < req_buf_end) {
            if (req_buf + sizeof(int32_t) > req_buf_end) {
                response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
                return;
            }
            field_length = *reinterpret_cast<const int32_t*>(req_buf);
            req_buf += sizeof(int32_t);
            if (req_buf + field_length > req_buf_end) {
                response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
                return;
            }
            fields.emplace_back(::leveldb::Slice(req_buf, field_length));
            req_buf += field_length;
        }
        leveldb::Status s = local_server_->hmdel(user_key, fields);
        if (!s.ok()) {
            response->set_err(::bolero::proto::HashResponse::DB_ERROR);
            return;
        }
        response->set_err(::bolero::proto::HashResponse::OK);
    }

    static void* hash_handle(RegionServer* region,
                             const ::bolero::proto::HashRequest* request,
                             ::bolero::proto::HashResponse* response,
                             google::protobuf::Closure* done) {
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
            region->handle_hmset(request, response);
            SLOG(INFO, "HMSET");
            break;
        case ::bolero::proto::HashRequest::HMGET:
            region->handle_hmget(request, response);
            SLOG(INFO, "HMGET");
            break;
        case ::bolero::proto::HashRequest::HDEL:
            region->handle_hdel(request, response);
            SLOG(INFO, "HDEL");
            break;
        case ::bolero::proto::HashRequest::HMDEL:
            region->handle_hmdel(request, response);
            SLOG(INFO, "HMDEL");
            break;
        default:
            assert(false);
        }
        done->Run();
        return nullptr;
    }

    void RegionServer::hash_op(google::protobuf::RpcController* controller,
                               const ::bolero::proto::HashRequest* request,
                               ::bolero::proto::HashResponse* response,
                               google::protobuf::Closure* done) {
        hash_handle(this, request, response, done);
    }
}//namespace bolero

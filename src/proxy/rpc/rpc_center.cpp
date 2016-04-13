#include "rpc_center.h"

namespace bolero {
    ProxyConfig* RpcCenter::config_ = nullptr;
    static void encode_field_value(leveldb::Slice field, leveldb::Slice value, std::string* buffer) {
        uint32_t length = field.size();
        buffer->append(reinterpret_cast<char*>(&length), sizeof(uint32_t));
        buffer->append(field.data(), field.size());
        length = value.size();
        buffer->append(reinterpret_cast<char*>(&length), sizeof(uint32_t));
        buffer->append(value.data(), value.size());
    }
    bool RpcCenter::init() {
        if (config_ == nullptr) {
            return false;
        }
        client_ = new ::sofa::pbrpc::RpcClient(config_->default_client_opt());
        rpc_channel_ = new ::sofa::pbrpc::RpcChannel(client_, server_host_);
        stub_ = new ::bolero::proto::RegionServerProto_Stub(rpc_channel_);
        return true;
    }
    bool RpcCenter::hget(leveldb::Slice user_key, leveldb::Slice field, 
                         std::string* value, ::google::protobuf::Closure* done,
                         int64_t time_out) {
        ::bolero::proto::HashRequest* request = new ::bolero::proto::HashRequest;
        ::bolero::proto::HashResponse* response = new ::bolero::proto::HashResponse;

        request->set_operation(::bolero::proto::HashRequest::HGET);
        request->set_user_key(user_key.data(), user_key.size());
        request->set_req_batch(field.data(), field.size());
        if (user_key.size() <= 0 || field.size() <= 0) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return false;
        }

        ::sofa::pbrpc::RpcController* cntl = new ::sofa::pbrpc::RpcController;
        cntl->SetTimeout(time_out);
        stub_->hash_op(cntl, request, response, done);

        value->assign(response->res_batch());

        return true;
    }
    bool RpcCenter::hset(leveldb::Slice user_key, leveldb::Slice field, 
                         leveldb::Slice value, ::google::protobuf::Closure* done,
                         int64_t time_out) {
        ::bolero::proto::HashRequest* request = new ::bolero::proto::HashRequest;
        ::bolero::proto::HashResponse* response = new ::bolero::proto::HashResponse;

        request->set_operation(::bolero::proto::HashRequest::HSET);
        request->set_user_key(user_key.data(), user_key.size());
        if (user_key.size() <= 0 || field.size() <= 0) {
            response->set_err(::bolero::proto::HashResponse::BAD_ARGS);
            return false;
        }

        std::string tmp;
        encode_field_value(field, value, &tmp);
        request->set_req_batch(tmp);

        ::sofa::pbrpc::RpcController* cntl = new ::sofa::pbrpc::RpcController;
        cntl->SetTimeout(time_out);
        stub_->hash_op(cntl, request, response, done);

        return true;
    }
}//namespace bolero

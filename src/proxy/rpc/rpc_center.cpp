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
        return true;
    }
    bool RpcCenter::hget(leveldb::Slice user_key, leveldb::Slice field,
                         ::google::protobuf::Closure* done,
                         int64_t time_out) {
        ::bolero::proto::HashRequest* request = new ::bolero::proto::HashRequest;
        ::bolero::proto::HashResponse* response = new ::bolero::proto::HashResponse;

        request->set_operation(::bolero::proto::HashRequest::HGET);
        request->set_user_key(user_key.data(), user_key.size());
        request->set_req_batch(field.data(), field.size());

        ::sofa::pbrpc::RpcController* cntl = new ::sofa::pbrpc::RpcController;
        cntl->SetTimeout(time_out);
        get_stub(forward_center_->dispatch(user_key)->addr)->hash_op(cntl, request, response, done);

        return true;
    }
    bool RpcCenter::hset(leveldb::Slice user_key, leveldb::Slice field,
                         leveldb::Slice value, ::google::protobuf::Closure* done,
                         int64_t time_out) {
        ::bolero::proto::HashRequest* request = new ::bolero::proto::HashRequest;
        ::bolero::proto::HashResponse* response = new ::bolero::proto::HashResponse;

        request->set_operation(::bolero::proto::HashRequest::HSET);
        request->set_user_key(user_key.data(), user_key.size());

        std::string tmp;
        encode_field_value(field, value, &tmp);
        request->set_req_batch(tmp);

        ::sofa::pbrpc::RpcController* cntl = new ::sofa::pbrpc::RpcController;
        cntl->SetTimeout(time_out);
        get_stub(forward_center_->dispatch(user_key)->addr)->hash_op(cntl, request, response, done);
        if (done == nullptr) {
            delete request;
            delete response;
            delete cntl;
        }
        return true;
    }
    bool RpcCenter::hdel(leveldb::Slice user_key, leveldb::Slice field,
                         ::google::protobuf::Closure* done,
                         int64_t time_out) {
        ::bolero::proto::HashRequest* request = new ::bolero::proto::HashRequest;
        ::bolero::proto::HashResponse* response = new ::bolero::proto::HashResponse;

        request->set_operation(::bolero::proto::HashRequest::HDEL);
        request->set_user_key(user_key.data(), user_key.size());
        request->set_req_batch(field.data(), field.size());

        ::sofa::pbrpc::RpcController* cntl = new ::sofa::pbrpc::RpcController;
        cntl->SetTimeout(time_out);
        get_stub(forward_center_->dispatch(user_key)->addr)->hash_op(cntl, request, response, done);
        if (done == nullptr) {
            delete request;
            delete response;
            delete cntl;
        }
        return true;
    }
    bool RpcCenter::hmget(leveldb::Slice user_key, const std::vector<leveldb::Slice>& fields,
                          ::google::protobuf::Closure* done,
                          int64_t time_out) {
        ::bolero::proto::HashRequest* request = new ::bolero::proto::HashRequest;
        ::bolero::proto::HashResponse* response = new ::bolero::proto::HashResponse;

        request->set_operation(::bolero::proto::HashRequest::HMGET);
        request->set_user_key(user_key.data(), user_key.size());
        
        std::string batch_buf;
        int32_t field_length;
        for (leveldb::Slice field : fields) {
            field_length = field.size();
            batch_buf.append(reinterpret_cast<char*>(&field_length), sizeof(int32_t));
            batch_buf.append(field.data(), field.size());
        }
        request->set_req_batch(std::move(batch_buf));

        ::sofa::pbrpc::RpcController* cntl = new ::sofa::pbrpc::RpcController;
        cntl->SetTimeout(time_out);
        get_stub(forward_center_->dispatch(user_key)->addr)->hash_op(cntl, request, response, done);
        if (done == nullptr) {
            delete request;
            delete response;
            delete cntl;
        }
        return true;
    }
    bool RpcCenter::hmset(leveldb::Slice user_key,
                          const std::vector<std::pair<leveldb::Slice, leveldb::Slice>>& kvs,
                          ::google::protobuf::Closure* done,
                          int64_t time_out) {
        ::bolero::proto::HashRequest* request = new ::bolero::proto::HashRequest;
        ::bolero::proto::HashResponse* response = new ::bolero::proto::HashResponse;

        request->set_operation(::bolero::proto::HashRequest::HMSET);
        request->set_user_key(user_key.data(), user_key.size());
        
        std::string batch_buf;
        int32_t length;
        for (auto kv : kvs) {
            length = kv.first.size();
            batch_buf.append(reinterpret_cast<char*>(&length), sizeof(int32_t));
            batch_buf.append(kv.first.data(), kv.first.size());
            length = kv.second.size();
            batch_buf.append(reinterpret_cast<char*>(&length), sizeof(int32_t));
            batch_buf.append(kv.second.data(), kv.second.size());
        }
        request->set_req_batch(std::move(batch_buf));

        ::sofa::pbrpc::RpcController* cntl = new ::sofa::pbrpc::RpcController;
        cntl->SetTimeout(time_out);
        get_stub(forward_center_->dispatch(user_key)->addr)->hash_op(cntl, request, response, done);
        if (done == nullptr) {
            delete request;
            delete response;
            delete cntl;
        }
        return true;
    }
    bool RpcCenter::hmdel(leveldb::Slice user_key, const std::vector<leveldb::Slice>& fields,
                          ::google::protobuf::Closure* done,
                          int64_t time_out) {
        ::bolero::proto::HashRequest* request = new ::bolero::proto::HashRequest;
        ::bolero::proto::HashResponse* response = new ::bolero::proto::HashResponse;

        request->set_operation(::bolero::proto::HashRequest::HMDEL);
        request->set_user_key(user_key.data(), user_key.size());
        
        std::string batch_buf;
        int32_t field_length;
        for (leveldb::Slice field : fields) {
            field_length = field.size();
            batch_buf.append(reinterpret_cast<char*>(&field_length), sizeof(int32_t));
            batch_buf.append(field.data(), field.size());
        }
        request->set_req_batch(std::move(batch_buf));

        ::sofa::pbrpc::RpcController* cntl = new ::sofa::pbrpc::RpcController;
        cntl->SetTimeout(time_out);
        get_stub(forward_center_->dispatch(user_key)->addr)->hash_op(cntl, request, response, done);
        if (done == nullptr) {
            delete request;
            delete response;
            delete cntl;
        }
        return true;
    }
    shared_ptr<::bolero::proto::RegionServerProto_Stub> RpcCenter::get_stub(const std::string& addr) {
        auto iter = rpc_stubs_.find(addr);
        if (iter != rpc_stubs_.end()) {
            return iter->second;
        }
        shared_ptr<::sofa::pbrpc::RpcChannel> channel(new ::sofa::pbrpc::RpcChannel(client_, addr));
        /*
         * NOTICE
         * convert shared_ptr to raw pointer here.
         */
        shared_ptr<::bolero::proto::RegionServerProto_Stub> stub(new ::bolero::proto::RegionServerProto_Stub(&*channel));
        rpc_channels_.insert(make_pair(addr, channel));
        rpc_stubs_.insert(make_pair(addr, stub));
        return stub;
    }
}//namespace bolero

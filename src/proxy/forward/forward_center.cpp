#include "forward_center.h"

#include "proto/region_server_proto.pb.h"
#include "rpc/proxy_config.h"
#include "rpc/rpc_center.h"
#include "sofa/pbrpc/pbrpc.h"

#include <assert.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

namespace bolero {
    std::string printbuffer(ReadBuffer& buf, int offset) {
        std::string ret;
        for (int i = 0; i < buf.size(); ++i) {
            if (i == offset) {
                ret += " | ";
            }
            if (buf[i] == '\r') {
                ret += "\\r";
            } else if (buf[i] == '\n') {
                ret += "\\n";
            } else {
                ret += buf[i];
            }
        }
        return ret;
    }
    static inline void bolero_set_nonblock(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        assert(flags != -1);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
    static inline void bolero_set_tcpnodelay(int fd) {
        int nodelay = 1;
        setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));
    }
    static inline void bolero_set_reuseaddr(int fd) {
        int reuse = 1;
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    }
    static bool buffer_get_int(const ReadBuffer& buf, int& offset, int limit,
                               int* res) {
        *res = 0;
        while (offset < limit) {
            const char& cur = buf[offset++];
            if (cur < '0' || cur > '9') {
                if (cur == '\r' && buf[offset++] == '\n') {
                    return true;
                }
                return false;
            }
            *res *= 10;
            *res += cur - '0';
        }
        return false;
    }
    static bool buffer_get_string(const ReadBuffer& buf, int& offset, int length,
                                  std::string* ret) {
        while (length > 0) {
            *ret += buf[offset++];
            --length;
        }
        if (buf[offset++] != '\r') {return false;}
        if (buf[offset++] != '\n') {return false;}
        return true;
    }
    static void invoke_hget(struct ClientConnection* client,
                            const std::vector<std::string> params) {
        assert(params[0] == "hget");
        if (params.size() != 3) {
            client->write_buffer.append("-Bad parameters\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
            return;
        }
        ::bolero::proto::HashRequest* request = RpcCenter::new_hash_request();
        ::bolero::proto::HashResponse* response = RpcCenter::new_hash_response();
        bool status = client->rpc_center->hget(params[1], params[2], request, response);
        if (!status || !response->has_err() ||
            response->err() != ::bolero::proto::HashResponse::OK) {
            client->write_buffer.append("-Err\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
            delete request;
            delete response;
            return;
        }
        const std::string& res = response->res_batch();
        if (res.size() <= sizeof(int32_t)) {
            client->write_buffer.append("$-1\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
        } else {
            int32_t length = *reinterpret_cast<const int32_t*>(res.data());
            char tmp[20];
            sprintf(tmp, "$%d\r\n", length);
            client->write_buffer.append(tmp);
            client->write_buffer.append(res.data() + sizeof(int32_t), length);
            client->write_buffer.append("\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
        }
        delete request;
        delete response;
    }
    static void invoke_hmget(struct ClientConnection* client,
                             const std::vector<std::string> params) {
        assert(params[0] == "hmget");
        if (params.size() < 3) {
            std::string info("-Bad parameters\r\n");
            client->write_buffer.append(info);
            client->write_buffer.send_all(client->fd, client->write_offset);
        }
        ::bolero::proto::HashRequest* request = RpcCenter::new_hash_request();
        ::bolero::proto::HashResponse* response = RpcCenter::new_hash_response();
        std::vector<leveldb::Slice> fields;
        for (size_t i = 2; i < params.size(); ++i) {
            fields.emplace_back(leveldb::Slice(params[i]));
        }
        bool status = client->rpc_center->hmget(params[1], fields, request, response);
        if (!status || !response->has_err() ||
            response->err() != ::bolero::proto::HashResponse::OK) {
            client->write_buffer.append("-Err\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
            delete request;
            delete response;
            return;
        }
        int offset = 0;
        int length;
        const char* buf = response->res_batch().data();
        char tmp[20];
        sprintf(tmp, "*%d\r\n", static_cast<int>(fields.size()));
        client->write_buffer.append(tmp);
        size_t cnt = 0;
        int batch_size = static_cast<int>(response->res_batch().size());
        while (offset < batch_size) {
            length = *reinterpret_cast<const int32_t*>(buf + offset);
            offset += sizeof(int32_t);

            sprintf(tmp, "$%d\r\n", length);
            client->write_buffer.append(tmp);
            if (length > 0) {
                std::string data(buf + offset, length);
                client->write_buffer.append(data);
                offset += length;
                client->write_buffer.append("\r\n");
            }
            cnt += 1;
        }
        if (cnt != fields.size()) {
            client->write_buffer.clear(client->write_offset);
            client->write_buffer.append("-Err\r\n");
        }
        client->write_buffer.send_all(client->fd, client->write_offset);
        delete request;
        delete response;
    }
    static void invoke_hset(struct ClientConnection* client,
                            const std::vector<std::string> params) {
        SLOG(INFO, "handle HSET");
        if (params.size() != 4) {
            client->write_buffer.append("-Bad parameters\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
            return;
        }
        ::bolero::proto::HashRequest* request = RpcCenter::new_hash_request();
        ::bolero::proto::HashResponse* response = RpcCenter::new_hash_response();
        bool status = client->rpc_center->hset(params[1], params[2], params[3],
                                               request, response);
        if (!status || !response->has_err() ||
            response->err() != ::bolero::proto::HashResponse::OK) {
            client->write_buffer.append("-Err\r\n");
        } else {
            client->write_buffer.append("+OK\r\n");
        }
        client->write_buffer.send_all(client->fd, client->write_offset);
        delete request;
        delete response;
    }
    static void invoke_hmset(struct ClientConnection* client,
                             const std::vector<std::string> params) {
        SLOG(INFO, "handle HMSET");
        if (params.size() < 4 || params.size() & 1) {
            client->write_buffer.append("-Bad parameters\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
            return;
        }
        ::bolero::proto::HashRequest* request = RpcCenter::new_hash_request();
        ::bolero::proto::HashResponse* response = RpcCenter::new_hash_response();
        std::vector<std::pair<leveldb::Slice, leveldb::Slice>> kvs;
        for (size_t i = 2; i < params.size(); i += 2) {
            kvs.emplace_back(std::make_pair(leveldb::Slice(params[i]),
                                            leveldb::Slice(params[i + 1])));
        }
        bool status = client->rpc_center->hmset(params[1], kvs, request, response);
        if (!status || !response->has_err() ||
            response->err() != ::bolero::proto::HashResponse::OK) {
            client->write_buffer.append("-Err\r\n");
        } else {
            client->write_buffer.append("+OK\r\n");
        }
        client->write_buffer.send_all(client->fd, client->write_offset);
        delete request;
        delete response;
    }
    static void invoke_hdel(struct ClientConnection* client,
                            const std::vector<std::string> params) {
        SLOG(INFO, "handle HDEL");
        if (params.size() != 3) {
            client->write_buffer.append("-Bad parameters\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
            return;
        }
        ::bolero::proto::HashRequest* request = RpcCenter::new_hash_request();
        ::bolero::proto::HashResponse* response = RpcCenter::new_hash_response();
        bool status = client->rpc_center->hdel(params[1], params[2],
                                               request, response);
        if (!status || !response->has_err() ||
            response->err() != ::bolero::proto::HashResponse::OK) {
            client->write_buffer.append("-Err\r\n");
        } else {
            client->write_buffer.append("+OK\r\n");
        }
        client->write_buffer.send_all(client->fd, client->write_offset);
        delete request;
        delete response;
    }
    static void invoke_hmdel(struct ClientConnection* client,
                             const std::vector<std::string> params) {
        SLOG(INFO, "handle HMDEL");
        if (params.size() < 3) {
            client->write_buffer.append("-Bad parameters\r\n");
            client->write_buffer.send_all(client->fd, client->write_offset);
            return;
        }
        ::bolero::proto::HashRequest* request = RpcCenter::new_hash_request();
        ::bolero::proto::HashResponse* response = RpcCenter::new_hash_response();
        std::vector<leveldb::Slice> fields;
        for (size_t i = 2; i < params.size(); ++i) {
            fields.emplace_back(leveldb::Slice(params[i]));
        }
        bool status = client->rpc_center->hmdel(params[1], fields,
                                                request, response);
        if (!status || !response->has_err() ||
            response->err() != ::bolero::proto::HashResponse::OK) {
            client->write_buffer.append("-Err\r\n");
        } else {
            client->write_buffer.append("+OK\r\n");
        }
        client->write_buffer.send_all(client->fd, client->write_offset);
        delete request;
        delete response;
    }

    static bool buffer_get_param(ReadBuffer& buf, int& offset, std::string* ret) {
        int length = 0;
        char flag;
        flag = buf[offset++];
        if (flag != '$') {
            return false;
        }
        if (!buffer_get_int(buf, offset, buf.size(), &length)) {
            return false;
        }
        if (!buffer_get_string(buf, offset, length, ret)) {
            return false;
        }
        return true;
    }
    static bool invoke_msg(struct ClientConnection* client) {
        int status = client->buffer.read_message(client->fd);
        SLOG(INFO, "read return %d", status);
        //TODO DEBUG
        SLOG(INFO, "read %s", printbuffer(client->buffer, client->read_offset).data());
        if (status <= 0) {
            //TODO DELETE
            //event_free(client->read_ev);
            return false;
        }
        if (client->buffer[client->read_offset++] != '*') {
            SLOG(INFO, "assert star failed");
            return false;
        }
        int arg_count;
        if (!buffer_get_int(client->buffer, client->read_offset, client->buffer.size(),
                            &arg_count)) {
            SLOG(INFO, "assert getint failed");
            return false;
        }
        std::vector<std::string> params;
        for (int i = 0; i < arg_count; ++i) {
            std::string cur;
            if (!buffer_get_param(client->buffer, client->read_offset, &cur)) {
                return false;
            }
            params.push_back(std::move(cur));
        }
        std::string& command = params[0];
        if (command == "hget") {
            invoke_hget(client, params);
        } else if (command == "hmget") {
            invoke_hmget(client, params);
        } else if (command == "hset") {
            invoke_hset(client, params);
        } else if (command == "hmset") {
            invoke_hmset(client, params);
        } else if (command == "hdel") {
            invoke_hdel(client, params);
        } else if (command == "hmdel") {
            invoke_hmdel(client, params);
        }
        return true;
    }
    static void client_request_handle(evutil_socket_t client, short flags, void* arg) {
        struct ClientConnection* client_conn = reinterpret_cast<struct ClientConnection*>(arg);
        int status = invoke_msg(client_conn);
        SLOG(INFO, "client request %d", status);
        //TODO log
    }
    static void proxy_accept(evutil_socket_t listener, short flags, void* arg) {
        ForwardCenter* forward_center = reinterpret_cast<ForwardCenter*>(arg);
        struct event_base* base = forward_center->ev_base();
        struct sockaddr_in sin;
        socklen_t sock_len = sizeof(sin);
        int client_fd = accept(listener, reinterpret_cast<struct sockaddr*>(&sin),
                               &sock_len);
        if (client_fd < 0) {
            SLOG(ERROR, "Accept client connection failed.\n");
            return;
        }
        bolero_set_nonblock(client_fd);
        bolero_set_tcpnodelay(client_fd);
        SLOG(INFO, "Accept client connection %s: %d", inet_ntoa(sin.sin_addr),
             sin.sin_port);

        struct ClientConnection* client = new struct ClientConnection;
        client->fd = client_fd;

        struct event* client_rd_event = event_new(base, client_fd,
                                                  EV_READ | EV_ET | EV_PERSIST,
                                                  client_request_handle, client);
        client->read_ev = client_rd_event;
        client->rpc_center = forward_center->get_rpc_center();
        event_add(client_rd_event, nullptr);
    }
    bool ForwardCenter::start_listen() {
        _ev_base = event_base_new();
        assert(_ev_base != nullptr);

        assert(_config != nullptr);
        sockaddr_in proxy;
        proxy.sin_family = AF_INET;
        proxy.sin_addr.s_addr = htonl(INADDR_ANY);
        proxy.sin_port = htons(_config->proxy_port);

        _proxy_fd = socket(AF_INET, SOCK_STREAM, 0);
        bolero_set_nonblock(_proxy_fd);
        bolero_set_tcpnodelay(_proxy_fd);
        bolero_set_reuseaddr(_proxy_fd);
        if (bind(_proxy_fd, reinterpret_cast<struct sockaddr*>(&proxy),
                 sizeof(proxy)) == -1) {
            SLOG(FATAL, "Fail to bind socket on port:%d", _config->proxy_port);
            return false;
        }
        if (listen(_proxy_fd, _config->max_client_conn) < 0) {
            SLOG(FATAL, "Fail to listen port:%d", _config->proxy_port);
            return false;
        }
        SLOG(INFO, "Start to accept request on port %d", _config->proxy_port);

        struct event* listen_event = nullptr;
        listen_event = event_new(_ev_base, _proxy_fd, EV_READ | EV_PERSIST,
                                 proxy_accept, reinterpret_cast<void*>(this));
        event_add(listen_event, nullptr);
        event_base_dispatch(_ev_base);
        return true;
    }
    int ReadBuffer::read_message(int fd) {
        static int max_try = 5;
        int status = -1;
        int trycount = 0;
        int ret = 0;
        while (true) {
            if (bufs_.empty() || bufs_[bufs_.size() - 1].size() == buffer_size_) {
                char* buf = new char[buffer_size_];
                for (;;) {
                    status = read(fd, buf, buffer_size_);

                    if (status > 0) {
                        bufs_.emplace_back(::leveldb::Slice(buf, status));
                        ret += status;
                        break;
                    } else if (status == 0) {
                        return 0;
                    }
                    if (errno == EINTR) {
                        continue;
                    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        if (trycount++ > max_try) {
                            return ret;
                        }
                        continue;
                    } else {
                        return -1;
                    }
                }
            } else {
                ::leveldb::Slice end = bufs_[bufs_.size() - 1];
                char* buf = const_cast<char*>(end.data() + end.size());
                int limit = buffer_size_ - end.size();

                assert(limit > 0);

                for (;;) {
                    status = read(fd, buf, buffer_size_);

                    if (status > 0) {
                        bufs_.erase(bufs_.begin() + (bufs_.size() - 1));
                        const char* cur = end.data();
                        int cur_size = end.size() + status;
                        bufs_.emplace_back(::leveldb::Slice(cur, cur_size));
                        ret += status;
                        break;
                    } else if (status == 0) {
                        return 0;
                    }
                    if (errno == EINTR) {
                        continue;
                    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        if (trycount++ > max_try) {
                            return ret;
                        }
                        continue;
                    } else {
                        return -1;
                    }
                }
            }
        }
    }
    bool WriteBuffer::send_all(int fd, int& offset) {
        while (!datas_.empty()) {
            std::string& front = datas_.front();
            if (offset >= static_cast<int>(front.size())) {
                return false;
            }
            const char* buf = front.data() + offset;
            int limit = front.size() - offset;

            int status = write(fd, buf, limit);
            if (status == 0) {
                return true;
            } else if (status > 0) {
                if (limit == status) {
                    offset = 0;
                    datas_.pop();
                } else {
                    offset += status;
                }
            } else {
                if (errno == EINTR) {
                    continue;
                } else if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    //maybe other operation is more reasonable?
                    continue;
                }
            }
        }
        return true;
    }
}//namespace bolero

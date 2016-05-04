#include "forward_center.h"

#include "rpc/proxy_config.h"
#include "sofa/pbrpc/pbrpc.h"

#include <assert.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

namespace bolero {
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
    static int buffer_cmp(const ReadBuffer& buf, ::leveldb::Slice slice, int& offset, int length) {
        for (int i = 0; i < length; ++i) {
            if ()
        }
    }
    static std::string buffer_get_string(int& offset, int length) {
    }
    static void invoke_hget(envutil_socket_t client, struct event* ev,
                            const ReadBuffer& buf, int offset) {
        
    }
    static bool invoke_msg(envutil_socket_t client, struct event* ev,
                           const ReadBuffer& buf) {
        int length = 0;
        int offset = 0;
        if (buf[offset++] != '*') {
            return false;
        }
        length = buffer_get_int(buf, offset, )
    }
    static void client_request_handle(envutil_socket_t client, short flags, void* arg) {
        ReadBuffer buffer;
        int status = buffer.read_message(client);

        struct event* client_ev = reinterpret_cast<struct event*>(arg);

        if (status != 0) {
            SLOG(ERROR, "Fail to read data from client, connection is shutting down");
            close(client);

            event_del(client_ev);
            event_free(client_ev);
            return;
        }
        invoke_msg(client, client_ev, buf);
    }
    static void proxy_accept(evutil_socket_t listener, short flags, void* arg) {
        struct event_base* base = reinterpret_cast<event_base*>(arg);
        struct sockaddr_in sin;
        socklen_t sock_len = sizeof(sin);
        int client_fd = accept(listener, reinterpret_cast<struct sockaddr*>(&sin),
                               &sock_len);
        if (client_fd < 0) {
            SLOG(ERROR, "Accept client connection failed.\n");
            return;
        }
        SLOG(INFO, "Accept client connection %s: %d", inet_ntoa(sin.sin_addr),
             sin.sin_port);
        struct event* client_rd_event = event_new(base, client_fd, EV_READ | EV_ET,
                                                  client_request_handle, client_rd_event);
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
                                 proxy_accept, reinterpret_cast<void*>(_ev_base));
        event_add(listen_event, nullptr);
        event_base_dispatch(_ev_base);
        return true;
    }
    int ReadBuffer::read_message(int fd) {
        int status;
        ::leveldb::Slice end = bufs_[bufs_.size() - 1];
        char* buf = nullptr;
        char* rest = nullptr;
        if (end.size() == buffer_size_) {
            buf = end.data();
            rest = end.data() + end.size();
        } else {
            buf = new char[buffer_size_];
            rest = nullptr;
        }
        for (;;) {
            if (rest != nullptr) {
                int restsz = buffer_size_ - (rest - buf);
                status = read(fd, rest, restsz);

                assert(restsz > 0 && restsz >= status);

                if (status > 0 && restsz != status) {
                    restsz -= status;
                    rest += status;
                    bufs_[bufs_.size() - 1] = ::leveldb::Slice(buf,bufs_[bufs.size() - 1].size() + status);
                    continue;
                } else if (status > 0 && restsz == status) {
                    rest = nullptr;
                    buf = new char[buffer_size_];
                    bufs_[bufs_.size() - 1] = ::leveldb::Slice(buf,bufs_[bufs.size() - 1].size() + status);
                    continue;
                }
                if (status == 0) {
                    return 0;
                }
                if (errno = EINTR) {
                    continue;
                } else {
                    return -1;
                }
            } else {
                status = read(fd, buf, buffer_size_);
                if (status > 0 && status == buffer_size_) {
                    bufs_.push_back(::leveldb::Slice(buf, buffer_size_));
                    buf = new char[buffer_size_];
                    continue;
                } else if (status > 0 && status != buffer_size_) {
                    bufs_.push_back(::leveldb::Slice(buf, status));
                    rest = buf + status;
                    continue;
                }
                if (status == 0) {
                    delete[] buf;
                    return 0;
                }
                if (errno = EINTR) {
                    continue;
                } else {
                    return -1;
                }
            }
        }
    }
}//namespace bolero

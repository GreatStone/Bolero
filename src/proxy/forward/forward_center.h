#ifndef BOLERO_FORWARD_CENTER_H
#define BOLERO_FORWARD_CENTER_H

#include "regions.h"

#include <event2/event.h>
#include <pthread.h>
#include <vector>
#include <queue>

namespace bolero {
#ifdef BOLERO_CLUSTER_SIZE
    const int kHashPoolSize = BOLERO_CLUSTER_SIZE;
#else
    const int kHashPoolSize = 1024;
#endif

    class ProxyConfig;
    class RpcCenter;
    class ForwardCenter {
    public:
        ForwardCenter(ProxyConfig* proxy): _config(proxy), _regions_pool(),
                                           _regions_mutex(), _ev_base(nullptr),
                                           _proxy_fd(0), _rpc_center() {
            pthread_mutex_init(&_regions_mutex, nullptr);
        }
        ~ForwardCenter() {
            pthread_mutex_destroy(&_regions_mutex);
        }
        shared_ptr<struct Region> dispatch(::leveldb::Slice key) {
            return _regions_pool.hash_dispatch(key);
        }
        void region_loss_handle(struct Region* region);
        void region_insert_handle(struct Region* region);
        /*
         * NOTICE
         * This function should be invoked only once time.
         */
        bool start_listen();
        void set_rpc_center(shared_ptr<RpcCenter> rpc_center) {
            _rpc_center = rpc_center;
        }
        shared_ptr<RpcCenter> get_rpc_center() {
            return _rpc_center.lock();
        }
        struct event_base* ev_base() const {return _ev_base;}
    private:
        ProxyConfig* _config;
        HashPool<kHashPoolSize> _regions_pool;
        pthread_mutex_t _regions_mutex;
        struct event_base* _ev_base;
        int _proxy_fd;
        weak_ptr<RpcCenter> _rpc_center;
    };
    class ReadBuffer {
    public:
        ReadBuffer(): bufs_(){
        }
        ~ReadBuffer() {
            for (::leveldb::Slice buf : bufs_) {
                delete[] buf.data();
            }
        }
        /*
         * @return
         *  >0 as read_size
         *  0 as eof
         *  -1 as error
         */
        int read_message(int fd);
        int size() const {
            return buffer_size_ * (bufs_.size() - 1) + bufs_[bufs_.size() - 1].size();
        }
        void pophead(int size, int& offset) {
            int cnt = size / buffer_size_;
            for (int i = 0; i < cnt; ++i) {
                delete bufs_[i].data();
                bufs_.erase(bufs_.begin());
            }
            offset -= cnt * buffer_size_;
        }
        /*
         * Never check out of bound.
         */
        char& operator[] (int i) {
            return const_cast<char&>(bufs_[i >> 16].data()[i & size_mask_]);
        }
        const char& operator[] (int i) const {
            return bufs_[i >> 16].data()[i & size_mask_];
        }
    private:
        static const int buffer_size_ = 2 << 16;
        static const int size_mask_ = (2 << 16) - 1;
        std::vector<::leveldb::Slice> bufs_;

        ReadBuffer(const ReadBuffer&);
        ReadBuffer& operator=(const ReadBuffer&);
    };
    class WriteBuffer {
    public:
        WriteBuffer(): datas_() { }
        void append(const std::string& data) {datas_.push(std::move(data));}
        void append(const char* data, int length) {datas_.push(std::string(data, length));}
        void append(const char* data) {
            datas_.push(std::move(std::string(data)));
        }
        bool send_all(int fd, int& offset);
        void clear(int& offset) {
            while (!datas_.empty()) {
                datas_.pop();
            }
            offset = 0;
        }
    private:
        std::queue<std::string> datas_;

        WriteBuffer(const WriteBuffer&);
        WriteBuffer& operator=(const WriteBuffer&);
    };
    struct ClientConnection {
        ClientConnection(): rpc_center(nullptr), fd(-1), read_ev(nullptr),
                            buffer(), read_offset(0),
                            write_buffer(), write_offset(0) {}
        shared_ptr<RpcCenter> rpc_center;
        int fd;
        struct event* read_ev;
        ReadBuffer buffer;
        int read_offset;
        WriteBuffer write_buffer;
        int write_offset;
    };
}//namespace bolero

#endif

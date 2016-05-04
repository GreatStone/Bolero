#ifndef BOLERO_FORWARD_CENTER_H
#define BOLERO_FORWARD_CENTER_H

#include "regions.h"

#include <event2/event.h>
#include <pthread.h>

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
                                           _proxy_fd(0), _rpc_center(nullptr) {
            pthread_mutex_init(&_regions_mutex, nullptr);
        }
        ~ForwardCenter() {
            pthread_mutex_destroy(&_regions_mutex);
        }
        shared_ptr<struct Region> dispatch(::leveldb::Slice key) {
            return _regions_pool.hash_dispatch(key);
        }
        void set_rpc_center(shared_ptr<RpcCenter> rpc_center) {_rpc_center = rpc_center;}
        void region_loss_handle(struct Region* region);
        void region_insert_handle(struct Region* region);
        /*
         * NOTICE
         * This function should be invoked only once time.
         */
        bool start_listen();
    private:
        ProxyConfig* _config;
        HashPool<kHashPoolSize> _regions_pool;
        pthread_mutex_t _regions_mutex;
        struct event_base* _ev_base;
        int _proxy_fd;
        shared_ptr<RpcCenter> _rpc_center;
    };
    class ReadBuffer {
    public:
        ReadBuffer(): bufs_(){
        }
        ~ReadBuffer() {
            for (::leveldb::Slice buf : bufs) {
                delete buf.data();
            }
        }
        /*
         * @return
         *  0 as eof
         *  -1 as error
         */
        int read_message(int fd);
        int size() const {
            return buffer_size_ * (bufs_.size() - 1) + bufs_[bufs_.size() - 1].size();
        }
        /*
         * Never check out of bound.
         */
        char& operator[] (int i) {
            return bufs_[i >> 16].data()[i & size_mask_];
        }
        const char& operator[] (int i) const {
            return bufs_[i >> 16].data()[i & size_mask_];
        }
    private:
        static const int buffer_size_ = 2 << 16;
        static const int size_mask_ = (2 << 16) - 1;
        vector<::leveldb::Slice> bufs_;

        ReadBuffer(const ReadBuffer&);
        ReadBuffer& operator=(const ReadBuffer&);
    }
}//namespace bolero

#endif

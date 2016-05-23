#include <pthread.h>

namespace bolero {
    class MutexLocking {
    public:
        MutexLocking(pthread_mutex_t* mutex) : mutex_(mutex) {
            pthread_mutex_lock(mutex_);
        }
        ~MutexLocking() {
            pthread_mutex_unlock(mutex_);
        }
    private:
        pthread_mutex_t* mutex_;

        MutexLocking(const MutexLocking&);
        MutexLocking& operator = (const MutexLocking&);
    };
}

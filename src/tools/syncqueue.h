#ifndef RAYMARCHING_SYNCQUEUE_H
#define RAYMARCHING_SYNCQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace RayMarching {

    template<typename T>
    class SyncQueue {
    private:
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _cond;
    public:
        SyncQueue() = default;

        ~SyncQueue() = default;

        bool empty();

        void enqueue(T element);

        T deque();
    };

    template<typename T>
    bool SyncQueue<T>::empty() {
        std::lock_guard<std::mutex> lk(_mutex);
        return _queue.empty();
    }

    template<typename T>
    void SyncQueue<T>::enqueue(T element) {
        std::lock_guard<std::mutex> lk(_mutex);
        _queue.push(element);
        _cond.notify_one();
    }

    template<typename T>
    T SyncQueue<T>::deque() {
        std::unique_lock<std::mutex> lk(_mutex);
        _cond.wait(lk, [this] { return !this->_queue.empty(); });
        T val = _queue.front();
        _queue.pop();
        return val;
    }

}
#endif //RAYMARCHING_SYNCQUEUE_H

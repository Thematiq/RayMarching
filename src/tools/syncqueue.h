#ifndef RAYMARCHING_SYNCQUEUE_H
#define RAYMARCHING_SYNCQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace RayMarching {

    /**
     * \brief Basic synchronized Queue template
     *
     * Synchronized Queue responsible for Camera threads coordination
     * @tparam T Queue content typename
     */
    template<typename T>
    class SyncQueue {
    private:
        std::queue<T> _queue;                /**< Internal Queue */
        std::mutex _mutex;                   /**< Internal Mutex */
        std::condition_variable _cond;       /**< Internal condition variable */
    public:

        /**
         * SyncQueue default constructor
         */
        SyncQueue() = default;

        /**
         * SyncQueue default destructor
         */
        ~SyncQueue() = default;

        /**
         * Checks if queue has any content
         * @return True if internal queue is empty
         */
        bool empty();

        /**
         * Pushes element into the queue and informs single awaiting member
         * @param element item pushed into the queue
         */
        void enqueue(T element);

        /**
         * Gets and pops item from the SyncQueue. If internal queue is empty awaits for the content
         * @return SyncQueue front
         */
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

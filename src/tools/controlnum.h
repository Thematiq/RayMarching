#ifndef RAYMARCHING_CONTROLNUM_H
#define RAYMARCHING_CONTROLNUM_H

#include <mutex>
#include <condition_variable>

namespace RayMarching {

    /** \brief Atomic unsigned int with condition variable waiter
     *
     * Atomic unsigned int which can be changed only by one. Additionally allows for awaiting for a given number
     */
    class ControlNum {
    private:
        unsigned int _num;             /**< Internal integer */
        std::mutex _mutex;             /**< Object's mutex */
        std::condition_variable _cond; /**< Object's condition variable */
    public:
        /**
         * Default constructor
         */
        ControlNum() : _num(0), _mutex(), _cond() {};

        /**
         * Default destructor
         */
        ~ControlNum() = default;

        /**
         * Internal value getter
         * @return Current value of the Object
         */
        unsigned int get();

        /**
         * Internal value setter
         * @param val sets current value to the val
         */
        void set(unsigned int val);

        /**
         * Sets thread to the sleep until internal value is equal to val
         * @param val awaited value
         */
        void await_for(unsigned int val);

        /**
         * Decrement operator
         */
        void operator--();

        /**
         * Increment operator
         */
        void operator++();
    };
}

#endif //RAYMARCHING_CONTROLNUM_H

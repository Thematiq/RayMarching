#include "controlnum.h"

namespace RayMarching {

    unsigned int ControlNum::get() {
        std::lock_guard<std::mutex> lk(_mutex);
        return _num;
    }

    void ControlNum::set(unsigned int val) {
        std::lock_guard<std::mutex> lk(_mutex);
        _num = val;
        _cond.notify_all();
    }

    void ControlNum::await_for(unsigned int val) {
        std::unique_lock<std::mutex> lk(_mutex);
        _cond.wait(lk, [this, val] { return this->_num == val; });
    }

    void ControlNum::operator--() {
        std::lock_guard<std::mutex> lk(_mutex);
        _num--;
        _cond.notify_all();
    }

    void ControlNum::operator++() {
        std::lock_guard<std::mutex> lk(_mutex);
        _num++;
        _cond.notify_all();
    }
}
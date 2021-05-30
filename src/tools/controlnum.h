#ifndef RAYMARCHING_CONTROLNUM_H
#define RAYMARCHING_CONTROLNUM_H

#include <mutex>
#include <condition_variable>

class ControlNum {
private:
    unsigned int _num;
    std::mutex _mutex;
    std::condition_variable _cond;
public:
    ControlNum() : _num(0), _mutex(), _cond() {};
    ~ControlNum() = default;
    unsigned int get();
    void set(unsigned int val);
    void await_for(unsigned int val);
    void operator--();
    void operator++();
};

#endif //RAYMARCHING_CONTROLNUM_H

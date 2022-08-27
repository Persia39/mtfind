#ifndef MTFIND_QUEUETHREADWRAPPER_H
#define MTFIND_QUEUETHREADWRAPPER_H

#include <mutex>
#include <queue>

template<typename T>
class QueueThreadWrapper {
public:
    QueueThreadWrapper() = default;

    bool IsEmpty() const { return _queue.empty(); }

    void Push(const T &value);
    T Pop();

private:
    std::queue<T> _queue;
    std::mutex _mutex;
};

#include "QueueThreadWrapper.ipp"

#endif //MTFIND_QUEUETHREADWRAPPER_H

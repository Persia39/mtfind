

template<typename T>
void QueueThreadWrapper<T>::Push(const T &value) {
    const std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(value);
}

template<typename T>
T QueueThreadWrapper<T>::Pop() {
    const std::lock_guard<std::mutex> lock(_mutex);
    auto back_value {_queue.front()};
    _queue.pop();

    return back_value;
}
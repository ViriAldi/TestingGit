//
// Created by virialdi on 09.04.21.
//

#ifndef AKS_LAB3_PART2_SAFE_QUEUE_HPP
#define AKS_LAB3_PART2_SAFE_QUEUE_HPP

#include <mutex>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <atomic>
#include <tuple>

template<typename T>
class SafeQueue {
protected:
    std::deque<T> deque{};
    std::mutex m{};
    std::condition_variable added{}, removed{};
    std::atomic<size_t> producers{0};
    size_t max_size;
public:
    explicit SafeQueue(int max_length){
        max_size = max_length;
    }
    void push(const T &value){
        std::unique_lock<std::mutex> m_lock(m);
        while (deque.size() > max_size) {
            removed.wait(m_lock);
        }
        deque.push_back(value);
        added.notify_one();
    }

    T pop(){
        std::unique_lock<std::mutex> m_lock(m);
        while (deque.empty()){
            added.wait(m_lock);
        }
        auto result = deque.front();
        deque.pop_front();
        removed.notify_one();
        return result;
    }

    size_t size(){
        std::unique_lock<std::mutex> m_lock(m);
        return deque.size();
    }

    bool empty(){
        std::unique_lock<std::mutex> m_lock(m);
        return deque.empty();
    }

    void add_producer(){
        producers++;
    }

    void remove_producer(){
        producers--;
    }

    bool no_more(){
        return producers == 0;
    }

    bool end(){
        return producers == 0 && deque.empty();
    }
};

#endif //AKS_LAB3_PART2_SAFE_QUEUE_HPP

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <vector>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue{
 public:
    BlockingQueue(int capacity) : capacity_(capacity), space_(capacity + 1),
        start_(0), end_(0) {}
    //forbid copy and assignment
    BlockingQueue(const BlockingQueue<T> &other) = delete;
    BlockingQueue<T>& operator=(const BlockingQueue<T> &other) = delete;

    void Push(const T&data){
        std::unique_lock<std::mutex> lock(mutex_);
        while(is_full()){
            is_not_full_.wait(lock);
        }
        space_[end_] = data;
        end_++;
        end_ %= (capacity_ + 1);
        is_not_empty_.notify_one();
    }

    //return pop data
    T Pop(){
        std::unique_lock<std::mutex> lock(mutex_);
        while(is_empty()){
            is_not_empty_.wait(lock);
        }
        //get current data
        T head_data = space_[start_];
        start_++;
        start_ %= (capacity_+1);
        is_not_full_.notify_one();
        return head_data;
    }

 private:
    bool is_empty(){
        return end_ == start_;
    }
    bool is_full(){
        return (start_ - end_ + capacity_) % (capacity_+1) == 0;
    }
    std::mutex mutex_;
    std::condition_variable is_not_full_;
    std::condition_variable is_not_empty_;
    int start_;
    int end_;
    int capacity_;
    std::vector<T> space_;
};

#endif
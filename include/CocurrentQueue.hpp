#pragma once
#include <SpinLock.hpp>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <optional>

namespace Ltlog {
enum QueueState : uint8_t { kQueueIdel, kQueuePush, kQueuePop };

template <typename T>
class CocurrentQueue {
   public:
    void Push(T&& val) {
        spin_lock_.Lock(kQueueIdel, kQueuePush);
        queue_.emplace(std::move(val));
        spin_lock_.Unlock();
    }

    void Push(const T& val) {
        spin_lock_.Lock(kQueueIdel, kQueuePush);
        queue_.push(val);
        spin_lock_.Unlock();
    }

    std::optional<T> Pop()
    {
        std::optional<T> data;
        spin_lock_.Lock(kQueueIdel, kQueuePop);
        if(!queue_.empty())
        {
            data = std::move(queue_.front());
            queue_.pop();
        }
        spin_lock_.Unlock();
        return data;
    }

   private:
    SpinLock<QueueState> spin_lock_;
    std::queue<T> queue_;
};
}  // namespace Ltlog
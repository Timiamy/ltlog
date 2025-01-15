#include <atomic>
#include <chrono>
#include <thread>
#include <Utils.hpp>

namespace ltlog {

template <class T>
class SpinLock {
   public:
    SpinLock() = default;
    ~SpinLock() = default;
    SpinLock(const SpinLock&) = delete;
    SpinLock(SpinLock&&) noexcept = delete;
    SpinLock& operator=(const SpinLock&) = delete;
    SpinLock& operator=(SpinLock&&) noexcept = delete;

    void Lock(T expected, T desired) {
        static_assert(std::atomic<T>::is_always_lock_free,
                      "T type isn't lock free");
        last_state_ = expected;
        int times = 0;
        while (!state_.compare_exchange_weak(
            expected, desired, std::memory_order_acquire)) 
        {
            expected = last_state_;
            if (++times > 10) {
                times = 0;
                NanoSleep(1);
            }
        }
    }

    void Unlock() {
        state_.exchange(last_state_, std::memory_order_release);
    }

   private:
    T last_state_;
    std::atomic<T> state_;
};
}  // namespace ltlog
/**
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#include "common.hpp"

void sleep(size_t millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

lock_handler lock_init()
{
    return new std::mutex;
}

void lock_deinit(lock_handler lck)
{
    delete lck;
}

void lock_get(lock_handler lck)
{
    lck->lock();
}

void lock_put(lock_handler lck)
{
    lck->unlock();
}

signal_handler signal_init()
{
    return new std::condition_variable();
}

void signal_deinit(signal_handler sig)
{
    delete sig;
}

int signal_wait(signal_handler sig, lock_handler lck, int timeout) // 0--succeed, -1 failed, -2 timeout.
{
    lck->unlock();
    int ret = 0;
    {
        std::unique_lock<std::mutex> lk(*lck);
        if (sig->wait_for(lk, std::chrono::milliseconds(timeout)) == std::cv_status::timeout)
        {
            ret = -2;
        }
    }
    lck->lock();
    return ret;
}

void signal_wakeup(signal_handler sig)
{
    sig->notify_one();
}


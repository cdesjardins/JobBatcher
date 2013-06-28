#ifndef CNTSEM_H
#define CNTSEM_H
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

class Barrier
{
    volatile unsigned int _count;
    boost::mutex _mutex;
    boost::condition_variable _condition;

public:
    explicit Barrier(unsigned int initial_count = 0) 
       : _count(initial_count),
         _mutex(), 
         _condition()
    {
    }

    void give()
    {
        boost::unique_lock<boost::mutex> lock(_mutex);
        _count++;
        _condition.notify_one(); 
    }

    void take(int num)
    {
        boost::unique_lock<boost::mutex> lock(_mutex);
        _count = -num;
    }

    void wait()
    {
        boost::unique_lock<boost::mutex> lock(_mutex);
        while (_count < 0)
        {
             _condition.wait(lock);
        }
    }
};

#endif

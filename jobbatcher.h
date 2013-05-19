#ifndef JOBBATCHER_H
#define JOBBATCHER_H
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "cntsem.h"

template <class TClass>
class JobBatcher
{
public:

    JobBatcher(int poolSize)
    {
        _pWork.reset(new boost::asio::io_service::work(_ioService));
        for ( int i = 0; i < poolSize; ++i)
        {
            _threadGroup.create_thread(boost::bind(&boost::asio::io_service::run, &_ioService));
        }
    }

    ~JobBatcher()
    {
        _pWork.reset();
        _threadGroup.join_all();
    }

    // this will leave immediately
    template <typename TFunc>
    void enqueue(TFunc fun)
    {
        _ioService.post(boost::bind(&JobBatcher::execute<TFunc>, this, fun));
    }

    void waitForBatch()
    {
        _sem.wait();
    }

    void initBatch(int num)
    {
        _sem.setCount(num);
    }

private:

    template <typename TFunc>
    void execute(TFunc fun)
    {
        fun();
        _sem.signal();
    }

    boost::asio::io_service _ioService;
    boost::shared_ptr<boost::asio::io_service::work> _pWork;

    CountingSemaphore _sem;
    boost::thread_group _threadGroup;
};
#endif

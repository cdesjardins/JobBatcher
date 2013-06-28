#ifndef JOBBATCHER_H
#define JOBBATCHER_H
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "barrier.h"

class JobBatcher
{
public:
    static boost::shared_ptr<JobBatcher> createJobPool(int poolSize)
    {
        boost::shared_ptr<JobBatcher> ret(new JobBatcher());
        ret->_pWork.reset(new boost::asio::io_service::work(ret->_ioService));
        for (int i = 0; i < poolSize; ++i)
        {
            ret->_threadGroup.create_thread(boost::bind(&boost::asio::io_service::run, &ret->_ioService));
        }
        return ret;
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
        _barrier.wait();
    }

    void initBatch(int num)
    {
        _barrier.take(num);
    }

private:
    /* Use the createJobPool function! */
    JobBatcher()
    {
    }


    template <typename TFunc>
    void execute(TFunc fun)
    {
        fun();
        _barrier.give();
    }

    boost::asio::io_service _ioService;
    boost::shared_ptr<boost::asio::io_service::work> _pWork;

    Barrier _barrier;
    boost::thread_group _threadGroup;
};
#endif

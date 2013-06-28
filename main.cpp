#include <iostream>
#include <boost/bind.hpp>
#include <boost/bind/protect.hpp>
#include <boost/thread/barrier.hpp>
#include <math.h>
#include "jobbatcher.h"

class TesterClass
{
public:
    TesterClass()
    {
        std::cout<<"Create new tester"<<std::endl;
        _data = 0;
    }
    void runSomeStuff()
    {
        int data = _data++;
        long number1 = 1;
        long number2 = 10000000;
        long counter;
        double result = 0.0;

        for (counter = number1; counter <= number2; counter++)
        {
            result = result + sqrt((double)counter);
        }
        std::cout<<"Something is running here "<<data<<" "<<result<<std::endl;
    }
private:
    int _data;
};

int  main()
{
    boost::shared_ptr<JobBatcher> bh = JobBatcher::createJobPool(4);
    for (int x = 0; x < 10; x++)
    {
        TesterClass t;
        bh->initBatch(100);
        for (int i = 0; i < 100; i++)
        {
            bh->enqueue(boost::protect(boost::bind(&TesterClass::runSomeStuff, &t)));
        }
        bh->waitForBatch();
    }
    return 0;
}

#include "semaphore_util.h"
#include <boost/thread.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/timer.hpp>

#ifdef ENABLE_SEMAPHORE_TEST

BOOST_AUTO_TEST_CASE(Semaphore) {
  auto sem = BASE_THREAD::CreateSemaphore();
  boost::thread thread1([sem_pointer = sem.get()]{
    boost::this_thread::sleep(boost::posix_time::seconds(5));
    sem_pointer->Signal();
    });
  sem->Wait();

  sem->Wait(2000);

  boost::thread thread2([sem_pointer = sem.get()]{
    boost::this_thread::sleep(boost::posix_time::seconds(5));
    sem_pointer->Signal();
    });

  sem->Wait(10000);
}
#endif

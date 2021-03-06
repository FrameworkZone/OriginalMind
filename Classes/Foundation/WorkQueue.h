/****************************************************************************
  Copyright (c) 2014-2015 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_WorkQueue_h
#define Foundation_WorkQueue_h

#include <mutex>
#include <queue>
#include <list>
#include <memory>
#include <thread>
#include "Runnable.h"
#include "Singleton.h"

namespace Foundation {

/** 
 * A simple work queue.
 */

class WorkQueue : public Singleton<WorkQueue>
{
public:
    /**
     * @brief Do work, for thread，see WorkQueue::createThreads.
     */
    void run();

    /** 
     * @brief Add work.
     * @param[in] pWork  Worker.
     */
    void addWork(Runnable* pWork);
protected:
    /**
     * @brief Construct a work queue, the thread count is (hardware_concurrency *2).
     */
    WorkQueue();
    ~WorkQueue();
private:
    friend class Singleton<WorkQueue> ;
    void createThreads(size_t nThreadCount);
private:
    std::recursive_mutex                    m_lock;

    std::queue<Runnable*>                   m_works;

    std::list<std::shared_ptr<std::thread> > m_threads;
};

} // namespace Foundation

#endif // Foundation_WorkQueue_h



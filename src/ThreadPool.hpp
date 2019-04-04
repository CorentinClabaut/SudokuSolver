#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace sudoku
{

class ThreadPool
{
    using AsioWorker = std::unique_ptr<boost::asio::io_service::work>;

public:
    ThreadPool(int threads) :
        m_Service(),
        m_ServiceWorker(new AsioWorker::element_type(m_Service))
    {
        for (int i = 0; i < threads; ++i)
        {
            auto worker = [this] { return m_Service.run(); };
            m_ThreadGroup.add_thread(new boost::thread(worker));
        }
    }

    ~ThreadPool()
    {
        m_ServiceWorker.reset();
        m_ThreadGroup.join_all();
        m_Service.stop();
    }

    template<class F>
    void Enqueue(F f)
    {
        m_Service.post(f);
    }

private:
    boost::asio::io_service m_Service;
    AsioWorker m_ServiceWorker;
    boost::thread_group m_ThreadGroup;
};

} // namespace sudoku

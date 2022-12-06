#pragma once
#include "singleton.h"

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>


class CThreadPool final
	: public CSingleton<CThreadPool>
{
public:
	CThreadPool();
	virtual ~CThreadPool();

public:
	template <typename F, typename... Args>
	std::future<std::invoke_result_t<F, Args...>> EnqueueJob(F&& f, Args&&...args);

public:
	bool IsEmpty();
private:
	void WorkerThread();
private:
	size_t                            m_threadCount;
	std::vector<std::thread>          m_vecWorker;
	std::queue<std::function<void()>> m_queueJob;
	std::condition_variable           m_condition;
	std::mutex                        m_mutex;
};

template <typename F, typename ... Args>
std::future<std::invoke_result_t<F, Args...>> CThreadPool::EnqueueJob(F&& f, Args&&...args)
{
	using return_t = std::invoke_result_t<F, Args ...>;
	auto ptrJob = std::make_shared<std::packaged_task<return_t()>>(std::bind(std::forward<F>(f),
	                                                                         std::forward<Args>(args)...));
	std::future<return_t> resultFuture = ptrJob->get_future();
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queueJob.push([ptrJob]()
		{
			(*ptrJob)();
		});
	}
	m_condition.notify_one();

	return resultFuture;
}

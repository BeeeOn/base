#pragma once

#include <queue>

#include <Poco/AtomicCounter.h>
#include <Poco/Event.h>
#include <Poco/Mutex.h>

#include "loop/StoppableRunnable.h"
#include "util/AsyncExecutor.h"

namespace BeeeOn {

class SequentialAsyncExecutor :
	public StoppableRunnable,
	public AsyncExecutor {
public:
	typedef Poco::SharedPtr<SequentialAsyncExecutor> Ptr;

	SequentialAsyncExecutor();
	~SequentialAsyncExecutor();

	void invoke(std::function<void()> f) override;

	void run() override;
	void stop() override;

private:
	void execute(std::function<void()> task);
	std::queue<std::function<void()>> m_taskQueue;
	Poco::Event m_wakeupEvent;
	Poco::FastMutex m_queueMutex;
	Poco::AtomicCounter m_stopRequested;
};

}

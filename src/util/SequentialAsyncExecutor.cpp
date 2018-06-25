#include <Poco/Logger.h>
#include <Poco/ScopedLock.h>

#include "di/Injectable.h"
#include "util/SequentialAsyncExecutor.h"

BEEEON_OBJECT_BEGIN(BeeeOn, SequentialAsyncExecutor)
BEEEON_OBJECT_CASTABLE(AsyncExecutor)
BEEEON_OBJECT_CASTABLE(StoppableRunnable)
BEEEON_OBJECT_END(BeeeOn, SequentialAsyncExecutor)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SequentialAsyncExecutor::SequentialAsyncExecutor() :
	m_stopRequested(false)
{
}

SequentialAsyncExecutor::~SequentialAsyncExecutor()
{
	if (!m_taskQueue.empty()) {
		poco_warning(logger(),
			"deleting object with non empty queue, tasks count: "
			+ to_string(m_taskQueue.size()));
	}
}

void SequentialAsyncExecutor::invoke(std::function<void()> f)
{
	FastMutex::ScopedLock lock(m_queueMutex);
	m_taskQueue.push(f);
	m_wakeupEvent.set();
}

void SequentialAsyncExecutor::run()
{
	std::function<void()> task;

	while (!m_stopRequested) {
		ScopedLockWithUnlock<FastMutex> guard(m_queueMutex);

		if (m_taskQueue.empty()) {
			guard.unlock();
			m_wakeupEvent.wait();
			continue;
		}

		task = m_taskQueue.front();
		m_taskQueue.pop();

		guard.unlock();
		execute(task);
	}

	m_stopRequested = false;

	if (!m_taskQueue.empty()) {
		poco_warning(logger(), "exiting thread with non empty queue");
	}

	m_stoppedEvent.set();
}

void SequentialAsyncExecutor::execute(std::function<void()> task)
{
	try {
		task();
	}
	BEEEON_CATCH_CHAIN(logger())
}

void SequentialAsyncExecutor::stop()
{
	m_stopRequested = true;
	m_wakeupEvent.set();

	m_stoppedEvent.wait();
}

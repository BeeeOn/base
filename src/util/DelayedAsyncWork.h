#pragma once

#include <functional>

#include <Poco/Event.h>
#include <Poco/Logger.h>
#include <Poco/RunnableAdapter.h>
#include <Poco/Thread.h>

#include "util/AbstractAsyncWork.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Implementation of the AsyncWork interface that executes
 * a given function once after the the given delay unless it is
 * cancelled.
 */
template <typename Result = Poco::Void>
class DelayedAsyncWork : public AbstractAsyncWork<Result>, Loggable {
public:
	typedef Poco::SharedPtr<DelayedAsyncWork<Result>> Ptr;

	DelayedAsyncWork(
		std::function<void()> f,
		const Poco::Timespan &delay);
	DelayedAsyncWork(
		std::function<void()> f,
		std::function<void()> cancelled,
		const Poco::Timespan &delay);

	bool tryJoin(const Poco::Timespan &timeout) override;
	void cancel() override;

protected:
	void run();

private:
	std::function<void()> m_f;
	std::function<void()> m_cancelled;
	Poco::Timespan m_delay;
	Poco::Event m_event;
	Poco::RunnableAdapter<DelayedAsyncWork> m_runnable;
	Poco::Thread m_thread;
};

template <typename Result>
DelayedAsyncWork<Result>::DelayedAsyncWork(
		std::function<void()> f,
		const Poco::Timespan &delay):
	DelayedAsyncWork<Result>(f, [](){}, delay)
{
}

template <typename Result>
DelayedAsyncWork<Result>::DelayedAsyncWork(
		std::function<void()> f,
		std::function<void()> cancelled,
		const Poco::Timespan &delay):
	m_f(f),
	m_cancelled(cancelled),
	m_delay(delay),
	m_runnable(*this, &DelayedAsyncWork<Result>::run)
{
	m_thread.start(m_runnable);
}

template <typename Result>
bool DelayedAsyncWork<Result>::tryJoin(const Poco::Timespan &timeout)
{
	return m_thread.tryJoin(timeout.totalMilliseconds());
}

template <typename Result>
void DelayedAsyncWork<Result>::cancel()
{
	m_event.set();
	m_thread.join();
}

template <typename Result>
void DelayedAsyncWork<Result>::run()
{
	if (m_event.tryWait(m_delay.totalMilliseconds())) {
		m_cancelled();
		return; // cancelled
	}

	try {
		m_f(); // not cancellable, must not sleep
	}
	BEEEON_CATCH_CHAIN(logger())
}

}

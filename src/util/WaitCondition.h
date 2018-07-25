#pragma once

#include <Poco/AtomicCounter.h>
#include <Poco/Condition.h>
#include <Poco/Mutex.h>
#include <Poco/Timespan.h>

namespace BeeeOn {

/**
 * @brief WaitCondition works as a barrier that waits until
 * some condition is met. When the condition is met and
 * broadcasted, all waiting threads are woken up.
 *
 * The WaitCondition can be configured to be non-repetitive.
 * I.e. after the first broadcast, all calls to wait or tryWait
 * would immediatelly return as successful.
 */
class WaitCondition {
public:
	WaitCondition(bool repeat = true);
	~WaitCondition();

	/**
	 * @brief Wait until the condition is broadcasted.
	 * Negative timeout would block infinitly (until broadcasted).
	 *
	 * @throws Poco::TimeoutException when timeout is exceeded
	 */
	void wait(const Poco::Timespan &timeout = -1);

	/**
	 * @brief Wait until the condition is broadcasted.
	 * Negative timeout would block infinitly (until broadcasted).
	 *
	 * @returns true if woken by broadcast, false when timeout has exceeded
	 */
	bool tryWait(const Poco::Timespan &timeout = -1);

	/**
	 * @brief Broadcast that the condition has been met.
	 */
	void broadcast();

protected:
	/**
	 * @brief Convert the given timeout into milliseconds.
	 * Negative value is converted to -1. Timeout shorter
	 * then 1 ms is converted to 1 ms.
	 */
	long toMilliseconds(const Poco::Timespan &timeout) const;

	/**
	 * @brief The actual waiting logic.
	 */
	bool doWait(long ms);

private:
	Poco::FastMutex m_lock;
	Poco::Condition m_condition;
	bool m_canRepeat;
	Poco::AtomicCounter m_broadcasted;
};

}

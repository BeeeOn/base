#include <Poco/Exception.h>

#include "util/PeriodicRunner.h"

using namespace Poco;
using namespace BeeeOn;

PeriodicRunner::PeriodicRunner():
	m_invoke(*this, &PeriodicRunner::onStart)
{
}

PeriodicRunner::~PeriodicRunner()
{
}

void PeriodicRunner::start(const Callback &callback)
{
	Timer::stop();

	m_callback = callback;
	Timer::start(m_invoke);
}

void PeriodicRunner::stop()
{
	Timer::stop();
}

void PeriodicRunner::setInterval(const Timespan &interval)
{
	if (interval.totalMilliseconds() <= 0)
		throw InvalidArgumentException("too small interval for period invocations");

	Timer::setPeriodicInterval(interval.totalMilliseconds());
}

void PeriodicRunner::onStart(Timer &)
{
	m_callback();
}

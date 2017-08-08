#include "util/FailDetector.h"

using namespace BeeeOn;
using namespace Poco;

FailDetector::FailDetector(const unsigned int failsTreshold):
	m_fails(0)
{
	m_failsTreshold = failsTreshold;
}

FailDetector::~FailDetector()
{
}

void FailDetector::fail()
{
	m_timeOfLastFail.update();

	if (!isFailed())
		++m_fails;
}

void FailDetector::success()
{
	m_fails = 0;
}

bool FailDetector::isFailed() const
{
	return (m_fails >= m_failsTreshold) && (m_failsTreshold > 0);
}

Timestamp FailDetector::timeOfLastFail() const
{
	return m_timeOfLastFail;
}

bool FailDetector::lastFailBefore(const Timespan &timeout) const
{
	return m_timeOfLastFail.isElapsed(timeout.totalMicroseconds());
}

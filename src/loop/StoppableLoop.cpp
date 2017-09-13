#include <Poco/Exception.h>
#include <Poco/Thread.h>
#include <Poco/Logger.h>

#include "loop/StoppableLoop.h"

using namespace Poco;
using namespace BeeeOn;

StoppableLoop::~StoppableLoop()
{
}

StoppableLoopAdapter::StoppableLoopAdapter(
		SharedPtr<StoppableRunnable> runnable):
	m_stopTimeout(10 * Timespan::SECONDS),
	m_runnable(runnable),
	m_thread(NULL)
{
	if (m_runnable.isNull())
		throw InvalidArgumentException("runnable must not be NULL");
}

StoppableLoopAdapter::~StoppableLoopAdapter()
{
	doStop();
}

void StoppableLoopAdapter::setStopTimeout(const Timespan &timeout)
{
	if (timeout < 1 * Timespan::MILLISECONDS)
		throw InvalidArgumentException("stopTimeout must be at least 1 ms");

	m_stopTimeout = timeout;
}

void StoppableLoopAdapter::start()
{
	if (m_thread != NULL && m_thread->isRunning())
		return;

	if (m_thread != NULL && !m_thread->isRunning())
		delete m_thread;

	m_thread = new Thread();
	m_thread->start(*m_runnable);
}

void StoppableLoopAdapter::stop()
{
	doStop();
}

void StoppableLoopAdapter::doStop()
{
	if (m_thread == NULL)
		return;

	if (m_thread->isRunning())
		m_runnable->stop();

	try {
		m_thread->join(m_stopTimeout.totalMilliseconds());
		delete m_thread;
	} catch (const TimeoutException &e) {
		/*
		 Do not delete the thread, there is no
		 portable way to stop it. If it contains
		 a bug, leave it as it is and just log
		 this fact.
		 */
		logger().log(e, __FILE__, __LINE__);
	}

	m_thread = NULL;
}

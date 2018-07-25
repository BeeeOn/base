#include <string>

#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "loop/LoopRunner.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;

BEEEON_OBJECT_BEGIN(BeeeOn, LoopRunner)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("runnables", &LoopRunner::addRunnable)
BEEEON_OBJECT_PROPERTY("loops", &LoopRunner::addLoop)
BEEEON_OBJECT_PROPERTY("autoStart", &LoopRunner::setAutoStart)
BEEEON_OBJECT_HOOK("done", &LoopRunner::autoStart)
BEEEON_OBJECT_END(BeeeOn, LoopRunner)

LoopRunner::LoopRunner():
	m_autoStart(false)
{
}

LoopRunner::~LoopRunner()
{
	stop();
}

void LoopRunner::addRunnable(SharedPtr<StoppableRunnable> runnable)
{
	addLoop(new StoppableLoopAdapter(runnable));
}

void LoopRunner::addLoop(SharedPtr<StoppableLoop> loop)
{
	FastMutex::ScopedLock guard(m_lock);

	m_loops.push_back(loop);
}

void LoopRunner::setAutoStart(bool enable)
{
	m_autoStart = enable;
}

void LoopRunner::stop()
{
	if (m_started.empty())
		return;

	logger().notice("stopping "
			+ to_string(m_started.size())
			+ " loops",
			__FILE__, __LINE__);

	FastMutex::ScopedLock guard(m_lock);

	stopAll(m_started);
}

void LoopRunner::stopAll(list<Stopper> &list)
{
	while (!list.empty()) {
		list.back().run();
		list.pop_back();
	}
}

void LoopRunner::start()
{
	FastMutex::ScopedLock guard(m_lock);

	for (auto &loop : m_loops) {
		try {
			loop->start();
			m_started.push_back(Stopper(loop));
		}
		BEEEON_CATCH_CHAIN_ACTION_RETHROW(logger(), stopAll(m_started));
	}

	logger().notice("started " + to_string(m_started.size()) + " loops",
			__FILE__, __LINE__);
}

void LoopRunner::autoStart()
{
	if (m_autoStart) {
		logger().information("auto-starting loops", __FILE__, __LINE__);
		start();
	}
}

LoopRunner::Stopper::Stopper():
	Loggable(typeid(LoopRunner))
{
}

LoopRunner::Stopper::Stopper(SharedPtr<StoppableLoop> loop):
	Loggable(typeid(LoopRunner)),
	m_loop(loop)
{
}

void LoopRunner::Stopper::run()
{
	if (m_loop.isNull())
		return;

	try {
		m_loop->stop();
	}
	BEEEON_CATCH_CHAIN(logger())
}

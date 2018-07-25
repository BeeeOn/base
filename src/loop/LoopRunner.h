#ifndef BEEEON_LOOP_RUNNER_H
#define BEEEON_LOOP_RUNNER_H

#include <list>

#include <Poco/Mutex.h>

#include "loop/StoppableRunnable.h"
#include "loop/StoppableLoop.h"
#include "util/Loggable.h"

namespace BeeeOn {

class LoopRunner : public StoppableLoop, public Loggable {
public:
	LoopRunner();
	~LoopRunner();

	void addRunnable(Poco::SharedPtr<StoppableRunnable> runnable);
	void addLoop(Poco::SharedPtr<StoppableLoop> loop);
	void setAutoStart(bool enable);

	void start() override;
	void stop() override;
	void autoStart();

protected:
	/**
	 * @brief Wrapper around StoppableLoop that allows to
	 * stop it from inside a thread.
	 */
	class Stopper : public Poco::Runnable, Loggable {
	public:
		Stopper();
		Stopper(Poco::SharedPtr<StoppableLoop> loop);

		void run() override;

	private:
		Poco::SharedPtr<StoppableLoop> m_loop;
	};

	void stopAll(std::list<Stopper> &list);

private:
	bool m_autoStart;
	Poco::FastMutex m_lock;
	std::list<Poco::SharedPtr<StoppableLoop>> m_loops;
	std::list<Stopper> m_started;
};

}

#endif

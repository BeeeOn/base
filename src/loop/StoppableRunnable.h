#pragma once

#include <Poco/Runnable.h>

namespace BeeeOn {

class StoppableRunnable : public Poco::Runnable {
public:
	virtual ~StoppableRunnable();

	/**
	 * Stop the runnable. The call should not block.
	 */
	virtual void stop() = 0;
};

}

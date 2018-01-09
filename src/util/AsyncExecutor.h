#pragma once

#include <functional>

#include <Poco/SharedPtr.h>

#include "util/Loggable.h"

namespace BeeeOn {

/**
 * Class for asynchronous executing tasks.
 */
class AsyncExecutor : public Loggable {
public:
	typedef Poco::SharedPtr<AsyncExecutor> Ptr;

	AsyncExecutor();
	~AsyncExecutor();

	/**
	 * Add task to queue for executing
	 */
	virtual void invoke(std::function<void()> f) = 0;
};

}

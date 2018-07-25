#pragma once

#include <Poco/Nullable.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/Void.h>

#include "util/Cancellable.h"

namespace BeeeOn {

/**
 * @brief AsyncWork is an interface allowing to wait until
 * an asynchronous operation finishes. It is also possible
 * to force-stop it by calling cancel().
 *
 * The AsyncWork can be parametrized to return a result.
 * Access to the result MUST be protected for safe parallel
 * access. The type of the result is based on the template
 * parameter.
 */
template <typename Result = Poco::Void>
class AsyncWork : public Cancellable {
public:
	typedef Poco::SharedPtr<AsyncWork> Ptr;

	virtual ~AsyncWork();

	/**
	 * @returns true when the time discovery process finishes,
	 * false when the timeout has exceeded
	 */
	virtual bool tryJoin(const Poco::Timespan &timeout) = 0;

	/**
	 * @brief If the asynchronous operation provides a result, this method
	 * gives access to it. Until the operation finishes, the result would
	 * be null. AsyncWork implementations with no result returns always
	 * null.
	 *
	 * @returns result of the asynchronous operation if any
	 */
	virtual Poco::Nullable<Result> result() const;
};

template <typename Result>
AsyncWork<Result>::~AsyncWork()
{
}

template <typename Result>
Poco::Nullable<Result> AsyncWork<Result>::result() const
{
	static const Poco::Nullable<Result> null;
	return null;
}

}

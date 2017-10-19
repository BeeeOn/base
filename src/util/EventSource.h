#ifndef BEEEON_EVENT_SOURCE_H
#define BEEEON_EVENT_SOURCE_H

#include <list>
#include <typeinfo>

#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>

#include "util/AsyncExecutor.h"
#include "util/ClassInfo.h"
#include "util/Loggable.h"
#include "util/Once.h"

namespace BeeeOn {

/**
 * @brief EventSource implements common logic for firing events to listeners.
 *
 * The EventSource can be inherited or used as member of another class (preferred)
 * to provide common logic for firing events. It registers listeners and fires
 * events via the provided AsyncExecutor.
 */
template <typename Listener>
class EventSource : protected Loggable {
public:
	EventSource();
	virtual ~EventSource();

	void setAsyncExecutor(Poco::SharedPtr<AsyncExecutor> executor);
	Poco::SharedPtr<AsyncExecutor> asyncExecutor() const;

	void addListener(typename Listener::Ptr listener);

	/**
	 * Fire the given instance of some event via the given method.
	 * The method argument should be pointer to some method of the
	 * Listener. Example:
	 *
	 * <pre>
	 * class SomeListener {
	 * public:
	 * 	virtual void onSome(const SomeEvent &e) = 0;
	 * };
	 *
	 * EventSource<SomeListener> source;
	 * source.addListener(...);
	 * source.setAsyncExecutor(...);
	 *
	 * SomeEvent e = ....;
	 * source.fireEvent(e, &SomeListener::onSome);
	 * </pre>
	 */
	template <typename Event, typename Method>
	void fireEvent(const Event &e, const Method &m);

private:
	Poco::SharedPtr<AsyncExecutor> m_executor;
	std::list<typename Listener::Ptr> m_listeners;
};

template <typename Listener>
EventSource<Listener>::EventSource()
{
}

template <typename Listener>
EventSource<Listener>::~EventSource()
{
}

template <typename Listener>
void EventSource<Listener>::setAsyncExecutor(Poco::SharedPtr<AsyncExecutor> executor)
{
	m_executor = executor;
}

template <typename Listener>
Poco::SharedPtr<AsyncExecutor> EventSource<Listener>::asyncExecutor() const
{
	return m_executor;
}

template <typename Listener>
void EventSource<Listener>::addListener(typename Listener::Ptr listener)
{
	m_listeners.push_back(listener);
}

template <typename Listener> template <typename Event, typename Method>
void EventSource<Listener>::fireEvent(const Event &e, const Method &m)
{
	static Once once;

	if (m_executor.isNull()) {
		once.execute([&]() {
			poco_warning(logger(), "no async executor is set");
		});
		return;
	}

	auto copy = m_listeners;

	m_executor->invoke([=]() {
		if (logger().debug()) {
			logger().debug("firing event " + ClassInfo(e).name(),
					__FILE__, __LINE__);
		}

		for (auto listener : copy) {
			try {
				(listener->*m)(e);
			}
			catch (const Poco::Exception &e) {
				logger().log(e, __FILE__, __LINE__);
				logger().error("failed to deliver event " + ClassInfo(e).name(),
						__FILE__, __LINE__);
			}
			catch (const std::exception &e) {
				logger().critical(e.what(), __FILE__, __LINE__);
				logger().critical("failed to deliver event " + ClassInfo(e).name(),
						__FILE__, __LINE__);
			}
			catch (...) {
				logger().critical("unknown failure when delivering event " + ClassInfo(e).name(),
						__FILE__, __LINE__);
			}
		}
	});
}

}

#endif

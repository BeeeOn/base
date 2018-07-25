#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>

#include "util/CancellableSet.h"
#include "util/ClassInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

CancellableSet::CancellableSet()
{
}

CancellableSet::~CancellableSet()
{
}

static string repr(const Cancellable::Ptr c)
{
	return ClassInfo::forPointer(c.get()).name()
		+ " (" + NumberFormatter::formatHex(reinterpret_cast<uintptr_t>(c.get())) + ")";
}

void CancellableSet::manage(Cancellable::Ptr c)
{
	FastMutex::ScopedLock guard(m_lock);

	if (logger().debug())
		logger().debug("manage " + repr(c), __FILE__, __LINE__);

	m_cancellables.emplace(c);
}

bool CancellableSet::unmanage(Cancellable::Ptr c)
{
	FastMutex::ScopedLock guard(m_lock);

	auto it = m_cancellables.find(c);
	if (it == m_cancellables.end())
		return false;

	if (logger().debug())
		logger().debug("unmanage " + repr(c), __FILE__, __LINE__);

	m_cancellables.erase(it);
	return true;
}

void CancellableSet::cancel()
{
	FastMutex::ScopedLock guard(m_lock);

	for (auto c : m_cancellables) {
		try {
			if (logger().debug())
				logger().debug("cancelling " + repr(c), __FILE__, __LINE__);

			c->cancel();
		}
		BEEEON_CATCH_CHAIN(logger())
	}

	m_cancellables.clear();
}

#ifndef BEEEON_HASHED_LOCK_H
#define BEEEON_HASHED_LOCK_H

#include <Poco/SharedPtr.h>

#include <vector>

namespace BeeeOn {

/**
 * HashedLock is useful to reduce overhead of fine-grained locking strategy.
 * If there are too many instances that should be locked during its processing
 * and maintaining a separate lock for each instance would be expensive, this
 * class can help to reduce this cost. The instances are sorted into buckets
 * that share a single lock. The sorting is performed via a hash function.
 * Thus, if the hash function has a low collision rate, the performance
 * of this strategy would be very close to the fine-grained locking.
 *
 * Common usage with Poco::FastMutex:
 *
 *   #define GOOD_PRIME 31
 *   HashedLock<FastMutex, MyType, MyTypeHash> lock(GOOD_PRIME);
 *
 *   startThread(lock, threadWork);
 *   startThread(lock, threadWork);
 *   startThread(lock, threadWork);
 *
 *   void threadWork(auto &lock)
 *   {
 *      MyType &type = obtainOne();
 *      FastMutex::ScopedLock guard(lock.find(type));
 *      ...
 *   }
 *
 * The probability of sharing a lock depends on the number of threads (usually
 * fixed) and the quality of the used hash function.
 */
template <typename LockType, typename ID, typename Hash = typename ID::Hash>
class HashedLock {
public:
	typedef LockType Lock;
	typedef Poco::SharedPtr<HashedLock<Lock, ID, Hash>> Ptr;

	/**
	 * Create count lock instances.
	 */
	HashedLock(unsigned int count);

	/**
	 * Find a lock for a given instance (or its identifier).
	 * The returned lock can be immediately used.
	 */
	Lock &find(const ID &id);

	/**
	 * Return lock at the given index.
	 */
	Lock &at(const unsigned int index);

	/**
	 * Return count of real locks.
	 */
	unsigned int size() const;

private:
	std::vector<Lock> m_lock;
};

template <typename Lock, typename ID, typename Hash>
HashedLock<Lock, ID, Hash>::HashedLock(unsigned int count):
	m_lock(count)
{
}

template <typename Lock, typename ID, typename Hash>
Lock &HashedLock<Lock, ID, Hash>::find(const ID &id)
{
	Hash hash;
	const unsigned int index = hash(id) % size();
	return at(index);
}

template <typename Lock, typename ID, typename Hash>
Lock &HashedLock<Lock, ID, Hash>::at(const unsigned int index)
{
	return m_lock[index];
}

template <typename Lock, typename ID, typename Hash>
unsigned int HashedLock<Lock, ID, Hash>::size() const
{
	return m_lock.size();
}

}

#endif

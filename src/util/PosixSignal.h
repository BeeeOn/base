#ifndef BEEEON_POSIX_SIGNAL_H
#define BEEEON_POSIX_SIGNAL_H

#include <string>

namespace BeeeOn {

/**
 * @brief The class implements working with POSIX signals.
 */
class PosixSignal {
private:
	PosixSignal();

public:
	/**
	 * Send signal to a process of the given pid.
	 * @param pid Process ID
	 * @param name Name of the signal to send (e.g. SIGUSR1)
	 */
	static void send(long pid, const std::string name);

protected:
	/**
	 * Translate name of a signal to its internal representation.
	 */
	static unsigned int byName(const std::string &name);

	static void send(long pid, unsigned int num);
};

}

#endif

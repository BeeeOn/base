#ifndef BEEEON_POSIX_SIGNAL_H
#define BEEEON_POSIX_SIGNAL_H

#include <functional>
#include <string>

namespace BeeeOn {

/**
 * @brief The class implements working with POSIX signals.
 */
class PosixSignal {
private:
	PosixSignal();

public:
	typedef std::function<void(unsigned int)> Handler;

	/**
	 * Send signal to a process of the given pid.
	 * @param pid Process ID
	 * @param name Name of the signal to send (e.g. SIGUSR1)
	 */
	static void send(long pid, const std::string name);

	/**
	 * Ignore the selected signal to not influence the current process (SIG_IGN).
	 * @param name Name of the signal to be ignored
	 */
	static void ignore(const std::string &name);

	/**
	 * Install a handler for the given signal name.
	 * @param name Name of the signal to handle
	 * @param handler Handler to be used
	 */
	static void handle(const std::string &name, Handler handler);

protected:
	/**
	 * Translate name of a signal to its internal representation.
	 */
	static unsigned int byName(const std::string &name);

	static void send(long pid, unsigned int num);
	static void ignore(unsigned int num);
	static void handle(unsigned int num, Handler handler);
};

}

#endif

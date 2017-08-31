#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstring>

#include <Poco/Exception.h>

#include "util/PosixSignal.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

PosixSignal::PosixSignal()
{
}

void PosixSignal::send(long pid, unsigned int num)
{
	pid_t epid = (pid_t) pid;

	if (epid <= 0)
		throw InvalidArgumentException("invalid pid given");

	if (kill(epid, num) < 0)
		throw IllegalStateException(string("failed to send signal ") + strerror(errno));
}

void PosixSignal::ignore(const unsigned int num)
{
	signal(num, SIG_IGN);
	if (errno)
		throw InvalidArgumentException(strerror(errno));
}

unsigned int PosixSignal::byName(const string &name)
{
	if (name == "SIGTERM")
		return SIGTERM;
	else if (name == "SIGINT")
		return SIGINT;
	else if (name == "SIGKILL")
		return SIGKILL;
	else if (name == "SIGUSR1")
		return SIGUSR1;
	else if (name == "SIGUSR2")
		return SIGUSR2;
	else if (name == "SIGCONT")
		return SIGCONT;
	else if (name == "SIGHUP")
		return SIGHUP;
	else
		throw InvalidArgumentException("unrecognized signal name");
}

void PosixSignal::send(long pid, const string name)
{
	send(pid, byName(name));
}

void PosixSignal::ignore(const string &name)
{
	ignore(byName(name));
}

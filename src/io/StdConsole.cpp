#include <iostream>

#include "di/Injectable.h"
#include "io/StdConsole.h"

BEEEON_OBJECT_BEGIN(BeeeOn, StdConsole)
BEEEON_OBJECT_CASTABLE(Console)
BEEEON_OBJECT_PROPERTY("eol", &StdConsole::setEol)
BEEEON_OBJECT_PROPERTY("skipEol", &StdConsole::setSkipEol)
BEEEON_OBJECT_PROPERTY("prompt", &StdConsole::setPrompt)
BEEEON_OBJECT_END(BeeeOn, StdConsole)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

StdConsoleSessionImpl::StdConsoleSessionImpl(Semaphore &semaphore):
	m_semaphore(semaphore)
{
}

StdConsoleSessionImpl::~StdConsoleSessionImpl()
{
	m_semaphore.set();
}

string StdConsoleSessionImpl::readUntil(const char c)
{
	return iosReadUntil(cin, c);
}

string StdConsoleSessionImpl::readBytes(const unsigned int length)
{
	return iosReadBytes(cin, length);
}

void StdConsoleSessionImpl::print(const string &text, bool newline)
{
	iosPrint(cout, text, newline);
}

bool StdConsoleSessionImpl::eof()
{
	return cin.eof();
}

StdConsole::StdConsole():
	m_semaphore(1),
	m_close(false)
{
}

void StdConsole::close()
{
	m_close = true;
}

ConsoleSessionImpl::Ptr StdConsole::openSession()
{
	m_semaphore.wait();

	if (m_close)
		return closedSession();

	return new StdConsoleSessionImpl(m_semaphore);
}

#include <Poco/Logger.h>

#include "util/ClassInfo.h"
#include "util/Loggable.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Loggable::Loggable():
	m_logger(NULL)
{
}

Loggable::Loggable(const ClassInfo &info):
	m_name(info.name()),
	m_logger(NULL)
{
}

Loggable::Loggable(const std::type_info &info):
	Loggable(ClassInfo(info))
{
}

Loggable::~Loggable()
{
}

void Loggable::setupLogger(Logger *logger) const
{
	if (m_logger == NULL) {
		const string name = m_name.empty()?
			ClassInfo::forPointer(this).name() : m_name;

		m_logger = logger == NULL?
			&Poco::Logger::get(name.c_str()) : logger;
	}
}

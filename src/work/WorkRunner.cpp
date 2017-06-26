#include <Poco/SingletonHolder.h>
#include <Poco/Exception.h>

#include "work/WorkRunner.h"
#include "work/WorkBackup.h"
#include "work/WorkExecutor.h"

using namespace Poco;
using namespace BeeeOn;

WorkRunner::WorkRunner(WorkScheduler &scheduler):
	m_scheduler(scheduler),
	m_executor(&NullWorkExecutor::instance()),
	m_backup(&EmptyWorkBackup::instance())
{
}

WorkRunner::~WorkRunner()
{
}

void WorkRunner::setExecutor(WorkExecutor *executor)
{
	m_executor = executor? executor : &NullWorkExecutor::instance();
}

void WorkRunner::setWork(Work::Ptr work)
{
	m_work = work;
}

void WorkRunner::setBackup(WorkBackup *backup)
{
	m_backup = backup? backup : &EmptyWorkBackup::instance();
}

void WorkRunner::destroySelf()
{
	delete this;
}

WorkRunnerFactory::~WorkRunnerFactory()
{
}

WorkRunner *NullWorkRunnerFactory::create(WorkScheduler &)
{
	throw NotImplementedException(__func__);
}

WorkRunnerFactory &NullWorkRunnerFactory::instance()
{
	static SingletonHolder<NullWorkRunnerFactory> singleton;
	return *singleton.get();
}

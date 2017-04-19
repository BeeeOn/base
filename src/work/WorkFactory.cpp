#include "work/WorkFactory.h"
#include "Debug.h"

using namespace BeeeOn;

WorkFactory::~WorkFactory()
{
}

std::string WorkFactory::identify(const std::type_info &info)
{
	return classDemangle(info.name());
}

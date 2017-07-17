#include "gwmessage/GWMessageType.h"

using namespace BeeeOn;

EnumHelper<GWMessageTypeEnum::Raw>::ValueMap &GWMessageTypeEnum::valueMap()
{
	static EnumHelper<GWMessageTypeEnum::Raw>::ValueMap valueMap = {
	};

	return valueMap;
}

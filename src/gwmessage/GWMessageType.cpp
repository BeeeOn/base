#include "gwmessage/GWMessageType.h"

using namespace BeeeOn;

EnumHelper<GWMessageTypeEnum::Raw>::ValueMap &GWMessageTypeEnum::valueMap()
{
	static EnumHelper<GWMessageTypeEnum::Raw>::ValueMap valueMap = {
		{GATEWAY_REGISTER, "gateway_register"},
		{GATEWAY_ACCEPTED, "gateway_accepted"},
		{GENERIC_RESPONSE, "generic_response"},
	};

	return valueMap;
}

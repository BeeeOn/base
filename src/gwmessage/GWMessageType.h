#ifndef BEEEON_GW_MESSAGE_TYPE_H
#define BEEEON_GW_MESSAGE_TYPE_H

#include "util/Enum.h"

namespace BeeeOn {

struct GWMessageTypeEnum {
	enum Raw {
		GATEWAY_REGISTER,
		GATEWAY_ACCEPTED,
		GENERIC_RESPONSE,
	};

	static EnumHelper<Raw>::ValueMap &valueMap();
};

typedef Enum<GWMessageTypeEnum> GWMessageType;

}

#endif

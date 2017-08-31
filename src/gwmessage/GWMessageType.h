#ifndef BEEEON_GW_MESSAGE_TYPE_H
#define BEEEON_GW_MESSAGE_TYPE_H

#include "util/Enum.h"

namespace BeeeOn {

struct GWMessageTypeEnum {
	enum Raw {
		GATEWAY_REGISTER,
	};

	static EnumHelper<Raw>::ValueMap &valueMap();
};

typedef Enum<GWMessageTypeEnum> GWMessageType;

}

#endif

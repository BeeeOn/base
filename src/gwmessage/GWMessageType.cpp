#include "gwmessage/GWMessageType.h"

using namespace BeeeOn;

EnumHelper<GWMessageTypeEnum::Raw>::ValueMap &GWMessageTypeEnum::valueMap()
{
	static EnumHelper<GWMessageTypeEnum::Raw>::ValueMap valueMap = {
		{GATEWAY_REGISTER, "gateway_register"},
		{GATEWAY_ACCEPTED, "gateway_accepted"},
		{GENERIC_RESPONSE, "generic_response"},
		{GENERIC_ACK, "generic_ack"},
		{NEW_DEVICE_REQUEST, "new_device_request"},
		{LAST_VALUE_REQUEST, "last_value_request"},
		{LAST_VALUE_RESPONSE, "last_value_response"},
		{RESPONSE_WITH_ACK, "response_with_ack"},
		{SENSOR_DATA_CONFIRM, "sensor_data_confirm"},
		{SENSOR_DATA_EXPORT, "sensor_data_export"},
	};

	return valueMap;
}

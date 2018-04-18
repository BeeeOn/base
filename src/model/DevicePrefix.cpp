#include "DevicePrefix.h"

using namespace BeeeOn;

DevicePrefixNamesInitializer::DevicePrefixNamesInitializer(
		const ValueMap &map):
	EnumNamesInitializer<DevicePrefix::Raw>(customNamesMap(map))
{
}

DevicePrefixNamesInitializer::NamesMap DevicePrefixNamesInitializer::customNamesMap(
		const ValueMap &map)
{
	return initNamesMap(map);
}

EnumHelper<DevicePrefixEnum::Raw>::ValueMap
&DevicePrefixEnum::valueMap()
{
	static EnumHelper<DevicePrefixEnum::Raw>::ValueMap valueMap = {
		{DevicePrefixEnum::PREFIX_INVALID, "Invalid"},
		{DevicePrefixEnum::PREFIX_FITPROTOCOL, "Fitprotocol"},
		{DevicePrefixEnum::PREFIX_PRESSURE_SENSOR, "PressureSensor"},
		{DevicePrefixEnum::PREFIX_VIRTUAL_DEVICE, "VirtualDevice"},
		{DevicePrefixEnum::PREFIX_VPT, "VPT"},
		{DevicePrefixEnum::PREFIX_OPENHAB, "OpenHAB"},
		{DevicePrefixEnum::PREFIX_BLUETOOTH, "Bluetooth"},
		{DevicePrefixEnum::PREFIX_BELKIN_WEMO, "BelkinWemo"},
		{DevicePrefixEnum::PREFIX_ZWAVE, "Z-Wave"},
		{DevicePrefixEnum::PREFIX_JABLOTRON, "Jablotron"},
		{DevicePrefixEnum::PREFIX_IQRF, "IQRF"},
		{DevicePrefixEnum::PREFIX_LEDS, "LEDs"},
		{DevicePrefixEnum::PREFIX_PHILIPS_HUE, "Philips Hue"},
		{DevicePrefixEnum::PREFIX_BEEWI, "BeeWi"},
	};

	return valueMap;
}

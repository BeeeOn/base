#pragma once

#include <list>
#include <string>

#include <Poco/Timespan.h>

#include "model/DeviceID.h"
#include "model/ModuleType.h"

namespace BeeeOn {

/**
 * @brief The class wraps information about a device's type.
 * It describes its properties and provided sensor types.
 */
class DeviceDescription {
public:
	DeviceDescription(
		const DeviceID &deviceID,
		const std::string &vendor,
		const std::string &productName,
		const std::list<ModuleType> &dataTypes,
		Poco::Timespan refreshTime = -1);

	DeviceID id() const;
	std::string vendor() const;
	std::string productName() const;
	std::list<ModuleType> dataTypes() const;

	/**
	 * Refresh time can be in one of three forms:
	 *
	 * - positive value - initial refresh time of the device
	 * - 0 - initially, device does not report data periodically,
	 *   it can be changed to a positive value
	 * - negative - refresh time is unsupported
	 *
	 * @returns refresh time of the device
	 */
	Poco::Timespan refreshTime() const;

	std::string toString() const;
	std::string toPrettyString() const;

private:
	static std::string normalizeName(const std::string& bytes);

private:
	DeviceID m_deviceID;
	std::string m_vendor;
	std::string m_productName;
	std::list<ModuleType> m_dataTypes;
	Poco::Timespan m_refreshTime;
};

}

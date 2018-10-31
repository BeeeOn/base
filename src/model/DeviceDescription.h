#pragma once

#include <list>
#include <string>

#include <Poco/Nullable.h>
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
	class Builder {
	public:
		Builder();

		Builder &id(const DeviceID &id);
		Builder &type(
			const std::string &vendor,
			const std::string &name);

		template <typename Container>
		Builder &modules(const Container &types)
		{
			m_modules.clear();

			for (const auto type : types)
				m_modules.emplace_back(type);

			return *this;
		}

		Builder &refreshTime(const Poco::Timespan &time);
		Builder &disabledRefreshTime();
		Builder &noRefreshTime();

		DeviceDescription build() const;

	private:
		Poco::Nullable<DeviceID> m_id;
		Poco::Nullable<std::string> m_vendor;
		Poco::Nullable<std::string> m_product;
		std::list<ModuleType> m_modules;
		Poco::Timespan m_refreshTime;
	};

	DeviceDescription();

	void setID(const DeviceID &id);
	DeviceID id() const;

	void setVendor(const std::string &vendor);
	std::string vendor() const;

	void setProductName(const std::string &name);
	std::string productName() const;

	void setDataTypes(const std::list<ModuleType> &types);
	std::list<ModuleType> dataTypes() const;

	void setRefreshTime(const Poco::Timespan &time);

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

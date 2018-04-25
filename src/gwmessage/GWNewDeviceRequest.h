#ifndef BEEEON_GW_MESSAGE_NEW_DEVICE_REQUEST_H
#define BEEEON_GW_MESSAGE_NEW_DEVICE_REQUEST_H

#include <list>
#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"
#include "model/DeviceDescription.h"
#include "model/DeviceID.h"
#include "model/ModuleType.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the gateway to the server after
 * discovering new device in the sensoric network.
 *
 * The message contains unique device identification, its product name along
 * with vendor, refresh time and types of device modules.
 *
 * Response to this request is the generic response GWResponse.
 */
class GWNewDeviceRequest : public GWRequest {
public:
	typedef Poco::SharedPtr<GWNewDeviceRequest> Ptr;

	GWNewDeviceRequest();
	GWNewDeviceRequest(const Poco::JSON::Object::Ptr object);

	void setDeviceID(const DeviceID &deviceID);
	DeviceID deviceID() const;

	void setProductName(const std::string &name);
	std::string productName() const;

	void setVendor(const std::string &vendor);
	std::string vendor() const;

	void setModuleTypes(const std::list<ModuleType> &types);
	std::list<ModuleType> moduleTypes() const;

	void setRefreshTime(const Poco::Timespan &time);
	Poco::Timespan refreshTime() const;

	void setDeviceDescription(const DeviceDescription &description);
	DeviceDescription deviceDescription() const;
};

}

#endif

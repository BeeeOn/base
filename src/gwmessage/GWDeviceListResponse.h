#ifndef BEEEON_GW_MESSAGE_DEVICE_LIST_RESPONSE_H
#define BEEEON_GW_MESSAGE_DEVICE_LIST_RESPONSE_H

#include <map>
#include <vector>

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWResponse.h"
#include "model/DeviceID.h"
#include "model/ModuleID.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent byt the server to the gateway as a response
 * to the GWDeviceListRequest.
 *
 * The message contains a list of paired devices with a specific prefix.
 */
class GWDeviceListResponse : public GWResponse {
public:
	typedef Poco::SharedPtr<GWDeviceListResponse> Ptr;

	GWDeviceListResponse();
	GWDeviceListResponse(const Poco::JSON::Object::Ptr object);

	void setDevices(const std::vector<DeviceID> &devices);
	std::vector<DeviceID> devices() const;

	void setModulesValues(
		const DeviceID &device,
		const std::map<ModuleID, double> &values);
	std::map<ModuleID, double> modulesValues(const DeviceID &device) const;
};

}

#endif

#pragma once

#include <vector>

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"
#include "model/DeviceDescription.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the gateway to the server after
 * discovering a device consisting of a group of subdevices in the sensoric
 * network. Example of the device may be VPT regulator that consists of
 * 4 zones and a boiler. Vendor name stored in message has to be same
 * for all devices.
 *
 * Response to this request is the generic response GWResponse.
 */
class GWNewDeviceGroupRequest : public GWRequest {
public:
	typedef Poco::SharedPtr<GWNewDeviceGroupRequest> Ptr;

	GWNewDeviceGroupRequest();
	GWNewDeviceGroupRequest(const Poco::JSON::Object::Ptr object);

	void setGroupName(const std::string& groupName);
	std::string groupName() const;

	void setVendor(const std::string& vendor);
	std::string vendor() const;

	void addDeviceDescription(const DeviceDescription& deviceDescription);
	std::vector<DeviceDescription> deviceDescriptions() const;
};

}

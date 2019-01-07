#pragma once

#include <map>

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"
#include "model/DeviceID.h"
#include "model/RefreshTime.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the server to the gateway, intended
 * to inform gateway that the user accepted the discovered device. It means
 * request to pair the device with the gateway.
 */
class GWDeviceAcceptRequest : public GWRequest, Loggable {
public:
	typedef Poco::SharedPtr<GWDeviceAcceptRequest> Ptr;

	GWDeviceAcceptRequest();
	GWDeviceAcceptRequest(const Poco::JSON::Object::Ptr object);

	void setDeviceID(const DeviceID &deviceID);
	DeviceID deviceID() const;

	void setRefresh(const RefreshTime &refresh);
	RefreshTime refresh() const;

	void setProperties(
		const std::map<std::string, std::string> &properties);
	std::map<std::string, std::string> properties() const;
};

}

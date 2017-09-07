#ifndef BEEEON_GW_MESSAGE_DEVICE_ACCEPT_REQUEST_H
#define BEEEON_GW_MESSAGE_DEVICE_ACCEPT_REQUEST_H

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"
#include "model/DeviceID.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the server to the gateway, intended
 * to inform gateway that the user accepted the discovered device. It means
 * request to pair the device with the gateway.
 */
class GWDeviceAcceptRequest : public GWRequest {
public:
	typedef Poco::SharedPtr<GWDeviceAcceptRequest> Ptr;

	GWDeviceAcceptRequest();
	GWDeviceAcceptRequest(const Poco::JSON::Object::Ptr object);

	void setDeviceID(const DeviceID &deviceID);
	DeviceID deviceID() const;
};

}

#endif

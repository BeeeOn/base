#ifndef BEEEON_GW_MESSAGE_UNPAIR_REQUEST_H
#define BEEEON_GW_MESSAGE_UNPAIR_REQUEST_H

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"
#include "model/DeviceID.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the server to the gateway, intended
 * to unpair a paired device from the gateway.
 */
class GWUnpairRequest : public GWRequest {
public:
	typedef Poco::SharedPtr<GWUnpairRequest> Ptr;

	GWUnpairRequest();
	GWUnpairRequest(const Poco::JSON::Object::Ptr object);

	void setDeviceID(const DeviceID &deviceID);
	DeviceID deviceID() const;
};

}

#endif

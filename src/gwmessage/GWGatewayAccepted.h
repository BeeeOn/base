#ifndef BEEEON_GW_MESSAGE_GATEWAY_ACCEPTED_H
#define BEEEON_GW_MESSAGE_GATEWAY_ACCEPTED_H

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWMessage.h"

namespace BeeeOn {

/**
 * @brief Represents message sent by the server to the gateway
 * after successful registration.
 */
class GWGatewayAccepted : public GWMessage {
public:
	typedef Poco::SharedPtr<GWGatewayAccepted> Ptr;

	GWGatewayAccepted();
	GWGatewayAccepted(const Poco::JSON::Object::Ptr object);
};

}

#endif

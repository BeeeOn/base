#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"

namespace BeeeOn {

/**
 * @brief Represents a message to check connectivity. It is primary intended
 * to be sent by the server to the gateway.
 *
 * Response to this message is always generic response GWResponse with status
 * SUCCESS or nothing.
 */
class GWPingRequest : public GWRequest {
public:
	typedef Poco::SharedPtr<GWPingRequest> Ptr;

	GWPingRequest();
	GWPingRequest(const Poco::JSON::Object::Ptr object);
};

}

#ifndef BEEEON_GW_MESSAGE_LISTEN_REQUEST_H
#define BEEEON_GW_MESSAGE_LISTEN_REQUEST_H

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the server to the gateway, intended
 * to start 'listening mode' on the gateway. In this mode, gateway can discover
 * new devices from the sensoric network.
 *
 * The listening mode has typically a limited duration. After the duration
 * exceeds, the listening mode is automatically turned off.
 */
class GWListenRequest : public GWRequest {
public:
	typedef Poco::SharedPtr<GWListenRequest> Ptr;

	GWListenRequest();
	GWListenRequest(const Poco::JSON::Object::Ptr object);

	void setDuration(const Poco::Timespan &duration);
	Poco::Timespan duration() const;
};

}

#endif

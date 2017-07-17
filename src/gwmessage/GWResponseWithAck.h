#ifndef BEEEON_GW_MESSAGE_RESPONSE_WITH_ACK_H
#define BEEEON_GW_MESSAGE_RESPONSE_WITH_ACK_H

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWAck.h"
#include "gwmessage/GWResponse.h"

namespace BeeeOn {

/**
 * @brief Represents a response to a request message (subclass of a GWRequest),
 * that requires acknowledgement (GWAck).
 */
class GWResponseWithAck : public GWResponse {
public:
	typedef Poco::SharedPtr<GWResponseWithAck> Ptr;

	GWResponseWithAck();
	GWResponseWithAck(const Poco::JSON::Object::Ptr object);

protected:
	GWResponseWithAck(const GWMessageTypeEnum::Raw &type);

public:
	/**
	 * @brief Creates and returns a corresponding GWAck message.
	 */
	GWAck::Ptr ack() const;
};

}

#endif

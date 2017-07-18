#ifndef BEEEON_GW_MESSAGE_LAST_VALUE_RESPONSE_H
#define BEEEON_GW_MESSAGE_LAST_VALUE_RESPONSE_H

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWResponse.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the server to the gateway as a response
 * to the GWLastValueRequest.
 *
 * The message contains the last known value of a device module.
 */
class GWLastValueResponse : public GWResponse {
public:
	typedef Poco::SharedPtr<GWLastValueResponse> Ptr;

	GWLastValueResponse();
	GWLastValueResponse(const Poco::JSON::Object::Ptr object);

	void setValue(const double value);
	double value() const;

	void setValid(const bool valid);
	bool valid() const;
};

}

#endif

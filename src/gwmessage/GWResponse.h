#ifndef BEEEON_GW_MESSAGE_RESPONSE_H
#define BEEEON_GW_MESSAGE_RESPONSE_H

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWMessage.h"
#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * @brief Represents a response to a request message (subclass of a GWRequest).
 * The GWResponse can be used alone as a generic response or inherited by a more
 * specific response object.
 *
 * The GWResponse contains unique identifier intended to match
 * a response to a proper request.
 */
class GWResponse : public GWMessage {
public:
	typedef Poco::SharedPtr<GWResponse> Ptr;

	enum Status {
		/**
		 * Request was accepted.
		 */
		ACCEPTED,
		/**
		 * Request completed successfully.
		 */
		SUCCESS,
		/**
		 * Request failed.
		 */
		FAILED,
	};

	GWResponse();
	GWResponse(const Poco::JSON::Object::Ptr object);

protected:
	GWResponse(const GWMessageTypeEnum::Raw &type);

public:
	/**
	 * @brief Set a unique identifier intended to match a proper request.
	 */
	void setID(const GlobalID &id);

	/**
	 * @brief Returns a unique identifier intended to match a proper request.
	 */
	GlobalID id() const;

	void setStatus(Status status);
	Status status() const;

	/**
	 * @brief Covert integer value to GWResponse::Status and return it.
	 */
	static Status convertStatus(const int value);
};

}

#endif

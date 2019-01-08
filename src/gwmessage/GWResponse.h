#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWMessage.h"

namespace BeeeOn {

class GWAck;

/**
 * @brief Represents a response to a request message (subclass of a GWRequest).
 * The GWResponse can be used alone as a generic response or inherited by a more
 * specific response object.
 *
 * The GWResponse contains unique identifier intended to match
 * a response to a proper request.
 *
 * The entry "status" is currently represented as integer. It is however possible
 * to use also string representation, any of:
 *
 * - "accepted" (0)
 * - "success" (1)
 * - "failed" (2)
 * - "success_partial" (3)
 *
 * The string representation should be preferred over numbers. The integer representation
 * is treated as obsolete.
 *
 * An example (generic) response:
 * <pre>
 * {
 *   "id": "b036b7f7-47a1-4bea-9fa3-6024e8263dcd",
 *   "message_type": "generic_response",
 *   "status": "success"
 * }
 * </pre>
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
		/**
		 * Request has generally succeeded but there were some
		 * non-fatal errors or warnings.
		 */
		SUCCESS_PARTIAL,
	};

	GWResponse();
	GWResponse(const Poco::JSON::Object::Ptr object);

protected:
	GWResponse(const GWMessageTypeEnum::Raw &type);

public:
	void setStatus(Status status);
	Status status() const;

	/**
	 * @brief Covert integer value to GWResponse::Status and return it.
	 */
	static Status convertStatus(const int value);

	void setAckExpected(bool expected);
	bool ackExpected() const;

	/**
	 * @brief Creates and returns a corresponding GWAck message.
	 */
	Poco::SharedPtr<GWAck> ack() const;
};

}

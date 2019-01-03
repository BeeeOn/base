#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"
#include "model/Severity.h"

namespace BeeeOn {

/**
 * @brief GWNoticeRequest allows a gateway to send a notice
 * about something that happen. It can report progress information,
 * error details, hints, etc. Each notice is defined by the following
 * properties:
 *
 * - at - time when the message (or source event) has occoured
 * - severity - describes importance of the message: info, warn or error
 * - key - key that defines the message (intended to be translated to client)
 * - context - context that can be used to construct the client message
 *
 * All message should be designed with multi-locale environment in mind.
 * Thus, the key and context are kept separately and should be tranformed
 * to the appropriate user message on the presentation layer.
 */
class GWNoticeRequest : public GWRequest {
public:
	typedef Poco::SharedPtr<GWNoticeRequest> Ptr;

	GWNoticeRequest();
	GWNoticeRequest(const Poco::JSON::Object::Ptr object);

	void setAt(const Poco::Timestamp &at);
	Poco::Timestamp at() const;

	void setSeverity(const Severity &severity);
	Severity severity() const;

	void setKey(const std::string &key);
	std::string key() const;

	void setContext(const Poco::JSON::Object::Ptr context);
	Poco::JSON::Object::Ptr context() const;
};

}

#include <string>

#include <Poco/Exception.h>

#include "gwmessage/GWAck.h"
#include "gwmessage/GWResponse.h"
#include "gwmessage/GWResponseWithAck.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GWResponse::GWResponse():
	GWMessage(GWMessageType::GENERIC_RESPONSE)
{
}

GWResponse::GWResponse(const GWMessageTypeEnum::Raw &type) :
	GWMessage(type)
{
}

GWResponse::GWResponse(const JSON::Object::Ptr object) :
	GWMessage(object)
{
}

void GWResponse::setStatus(Status status)
{
	json()->set("status", static_cast<int>(status));
}

GWResponse::Status GWResponse::status() const
{
	return convertStatus(json()->getValue<int>("status"));
}

GWResponse::Status GWResponse::convertStatus(const int value)
{
	switch (static_cast<Status>(value)) {
	case ACCEPTED:
	case SUCCESS:
	case FAILED:
		return static_cast<Status>(value);
	}

	throw InvalidArgumentException("invalid status: " + to_string(value));
}

void GWResponse::setAckExpected(bool expected)
{
	json()->set("ack_expected", expected);
}

bool GWResponse::ackExpected() const
{
	if (!json()->has("ack_expected"))
		return typeid(*this) == typeid(GWResponseWithAck);

	return json()->optValue<bool>("ack_expected", false);
}

GWAck::Ptr GWResponse::ack() const
{
	GWAck::Ptr ack(new GWAck);

	ack->setID(id());
	ack->setStatus(status());

	return ack;
}

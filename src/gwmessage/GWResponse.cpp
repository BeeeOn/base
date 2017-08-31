#include <string>

#include <Poco/Exception.h>

#include "gwmessage/GWResponse.h"

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

void GWResponse::setID(const GlobalID &id)
{
	json()->set("id", id.toString());
}

GlobalID GWResponse::id() const
{
	return GlobalID::parse(json()->getValue<string>("id"));
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
	default:
		throw InvalidArgumentException("invalid status: " + to_string(value));
	}
}


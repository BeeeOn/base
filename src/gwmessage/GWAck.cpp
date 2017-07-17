#include "gwmessage/GWAck.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GWAck::GWAck():
	GWMessage(GWMessageType::GENERIC_ACK)
{
}

GWAck::GWAck(const GWMessageTypeEnum::Raw &type) :
	GWMessage(type)
{
}

GWAck::GWAck(const JSON::Object::Ptr object) :
	GWMessage(object)
{
}

void GWAck::setID(const GlobalID &id)
{
	json()->set("id", id.toString());
}

GlobalID GWAck::id() const
{
	return GlobalID::parse(json()->getValue<string>("id"));
}

void GWAck::setStatus(GWResponse::Status status)
{
	json()->set("status", static_cast<int>(status));
}

GWResponse::Status GWAck::status() const
{
	return GWResponse::convertStatus(json()->getValue<int>("status"));
}

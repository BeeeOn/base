#include "gwmessage/GWMessageType.h"
#include "gwmessage/GWResponseWithAck.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GWResponseWithAck::GWResponseWithAck():
	GWResponse(GWMessageType::RESPONSE_WITH_ACK)
{
}

GWResponseWithAck::GWResponseWithAck(const GWMessageType::Raw &type):
	GWResponse(type)
{
}

GWResponseWithAck::GWResponseWithAck(const JSON::Object::Ptr object):
	GWResponse(object)
{
}

GWAck::Ptr GWResponseWithAck::ack() const
{
	GWAck::Ptr ack(new GWAck);

	ack->setID(id());
	ack->setStatus(status());

	return ack;
}

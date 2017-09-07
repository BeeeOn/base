#include "gwmessage/GWPingRequest.h"

using namespace Poco;
using namespace BeeeOn;

GWPingRequest::GWPingRequest():
	GWRequest(GWMessageType::PING_REQUEST)
{
}

GWPingRequest::GWPingRequest(const JSON::Object::Ptr object):
	GWRequest(object)
{
}

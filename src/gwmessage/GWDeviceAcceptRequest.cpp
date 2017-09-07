#include "gwmessage/GWDeviceAcceptRequest.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GWDeviceAcceptRequest::GWDeviceAcceptRequest():
	GWRequest(GWMessageType::DEVICE_ACCEPT_REQUEST)
{
}

GWDeviceAcceptRequest::GWDeviceAcceptRequest(const JSON::Object::Ptr object):
	GWRequest(object)
{
}

void GWDeviceAcceptRequest::setDeviceID(const DeviceID &deviceID)
{
	json()->set("device_id", deviceID.toString());
}

DeviceID GWDeviceAcceptRequest::deviceID() const
{
	return DeviceID::parse(json()->getValue<string>("device_id"));
}

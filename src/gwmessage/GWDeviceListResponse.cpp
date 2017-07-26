#include "gwmessage/GWDeviceListResponse.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GWDeviceListResponse::GWDeviceListResponse():
	GWResponse(GWMessageType::DEVICE_LIST_RESPONSE)
{
}

GWDeviceListResponse::GWDeviceListResponse(const JSON::Object::Ptr object) :
	GWResponse(object)
{
}

void GWDeviceListResponse::setDevices(const vector<DeviceID> &devices)
{
	JSON::Array::Ptr array(new JSON::Array);

	for (const auto &id : devices) {
		JSON::Object::Ptr object(new JSON::Object);
		object->set("device_id", id.toString());
		array->add(object);
	}

	json()->set("devices", array);
}

vector<DeviceID> GWDeviceListResponse::devices() const
{
	vector<DeviceID> devices;
	JSON::Array::Ptr array = json()->getArray("devices");

	for (size_t i = 0; i < array->size(); i++) {
		JSON::Object::Ptr object = array->getObject(i);

		devices.push_back(DeviceID::parse(object->getValue<string>("device_id")));
	}

	return devices;
}


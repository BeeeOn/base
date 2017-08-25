#include <string>

#include "gwmessage/GWSensorDataConfirm.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GWSensorDataConfirm::GWSensorDataConfirm():
	GWMessage(GWMessageType::SENSOR_DATA_CONFIRM)
{
}

GWSensorDataConfirm::GWSensorDataConfirm(const JSON::Object::Ptr object):
	GWMessage(object)
{
}

void GWSensorDataConfirm::setID(const GlobalID &id)
{
	json()->set("id", id.toString());
}

GlobalID GWSensorDataConfirm::id() const
{
	return GlobalID::parse(json()->getValue<string>("id"));
}

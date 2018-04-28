#include "gwmessage/GWNewDeviceRequest.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GWNewDeviceRequest::GWNewDeviceRequest():
	GWRequest(GWMessageType::NEW_DEVICE_REQUEST)
{
}

GWNewDeviceRequest::GWNewDeviceRequest(const JSON::Object::Ptr object):
	GWRequest(object)
{
}

void GWNewDeviceRequest::setDeviceID(const DeviceID &deviceID)
{
	json()->set("device_id", deviceID.toString());
}

DeviceID GWNewDeviceRequest::deviceID() const
{
	return DeviceID::parse(json()->getValue<string>("device_id"));
}

void GWNewDeviceRequest::setProductName(const string &name)
{
	json()->set("product_name", name);
}

string GWNewDeviceRequest::productName() const
{
	return json()->getValue<string>("product_name");
}

void GWNewDeviceRequest::setVendor(const string &vendor)
{
	json()->set("vendor", vendor);
}

string GWNewDeviceRequest::vendor() const
{
	return json()->getValue<string>("vendor");
}

void GWNewDeviceRequest::setModuleTypes(const list<ModuleType> &types)
{
	json()->set("module_types", serializeModuleTypes(types));
}

list<ModuleType> GWNewDeviceRequest::moduleTypes() const
{
	JSON::Array::Ptr arrayOfTypes = json()->getArray("module_types");

	return parseModuleTypes(arrayOfTypes);
}

void GWNewDeviceRequest::setRefreshTime(const Poco::Timespan &time)
{
	if (time < 0)
		json()->set("refresh_time", -1);
	else
		json()->set("refresh_time", time.totalSeconds());
}

Poco::Timespan GWNewDeviceRequest::refreshTime() const
{
	return json()->getValue<int>("refresh_time") * Timespan::SECONDS;
}

void GWNewDeviceRequest::setDeviceDescription(const DeviceDescription &description)
{
	setDeviceID(description.id());
	setVendor(description.vendor());
	setProductName(description.productName());
	setModuleTypes(description.dataTypes());
	setRefreshTime(description.refreshTime());
}

DeviceDescription GWNewDeviceRequest::deviceDescription() const
{
	DeviceDescription description(
		deviceID(),
		vendor(),
		productName(),
		moduleTypes(),
		refreshTime());

	return description;
}

JSON::Array::Ptr GWNewDeviceRequest::serializeModuleTypes(const list<ModuleType> &types)
{
	JSON::Array::Ptr arrayOfTypes(new JSON::Array);

	for (const auto &type : types) {
		JSON::Object::Ptr typeObject(new JSON::Object);
		typeObject->set("type", type.type().toString());

		JSON::Array::Ptr arrayOfAttributes(new JSON::Array);

		for (const auto &attribute : type.attributes()){
			JSON::Object::Ptr attributeObject(new JSON::Object);
			attributeObject->set("attribute", attribute.toString());

			arrayOfAttributes->add(Dynamic::Var(attributeObject));
		}

		typeObject->set("attributes", arrayOfAttributes);

		arrayOfTypes->add(Dynamic::Var(typeObject));
	}

	return arrayOfTypes;
}

list<ModuleType> GWNewDeviceRequest::parseModuleTypes(const JSON::Array::Ptr arrayOfTypes)
{
	list<ModuleType> types;

	for (size_t i = 0; i < arrayOfTypes->size(); i++) {
		const JSON::Object::Ptr typeObject = arrayOfTypes->getObject(i);

		ModuleType::Type type(ModuleType::Type::parse(
			typeObject->getValue<string>("type")));

		set<ModuleType::Attribute> attributes;

		JSON::Array::Ptr arrayOfAttributes = typeObject->getArray("attributes");

		for(size_t j = 0; j < arrayOfAttributes->size(); j++) {
			const JSON::Object::Ptr attributeObject = arrayOfAttributes->getObject(j);

			attributes.emplace(ModuleType::Attribute::parse(
				attributeObject->getValue<string>("attribute")));
		}

		types.push_back(ModuleType(type, attributes));
	}

	return types;
}

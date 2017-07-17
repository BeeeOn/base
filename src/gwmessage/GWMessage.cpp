#include "gwmessage/GWMessage.h"
#include "gwmessage/GWGatewayRegister.h"
#include "gwmessage/GWGatewayAccepted.h"
#include "gwmessage/GWResponse.h"
#include "util/JsonUtil.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

const string MESSAGE_TYPE_KEY = "message_type";

GWMessage::GWMessage(const GWMessageType::Raw &type):
	m_json(new JSON::Object)
{
	setType(type);
}

GWMessage::GWMessage(const JSON::Object::Ptr object):
	m_json(object)
{
}

GWMessage::~GWMessage()
{
}

GWMessageType GWMessage::type() const
{
	return GWMessageType::parse(m_json->getValue<string>(MESSAGE_TYPE_KEY));
}

std::string GWMessage::toString() const
{
	return Dynamic::Var::toString(m_json);
}

GWMessage::Ptr GWMessage::fromJSON(const std::string &json)
{
	return fromJSON(JsonUtil::parse(json));
}

GWMessage::Ptr GWMessage::fromJSON(Poco::JSON::Object::Ptr object)
{
	const GWMessageType &type = GWMessageType::parse(
		object->getValue<string>(MESSAGE_TYPE_KEY)
	);

	switch(type.raw()) {
	case GWMessageType::GATEWAY_REGISTER:
		return new GWGatewayRegister(object);
	case GWMessageType::GATEWAY_ACCEPTED:
		return new GWGatewayAccepted(object);
	case GWMessageType::GENERIC_RESPONSE:
		return new GWResponse(object);
	default:
		throw InvalidArgumentException(
			"unsupported message type" + type.toString());
	}
}

JSON::Object::Ptr GWMessage::json() const
{
	return m_json;
}

void GWMessage::setType(const GWMessageType &type)
{
	m_json->set(MESSAGE_TYPE_KEY, type.toString());
}

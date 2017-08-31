#include "gwmessage/GWRequest.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GWRequest::GWRequest(const GWMessageType::Raw &type):
	GWMessage(type)
{
}

GWRequest::GWRequest(const JSON::Object::Ptr object):
	GWMessage(object)
{
}

void GWRequest::setID(const GlobalID &id)
{
	json()->set("id", id.toString());
}

GlobalID GWRequest::id() const
{
	return GlobalID::parse(json()->getValue<string>("id"));
}

GWResponse::Ptr GWRequest::deriveGenericResponse(GWResponse::Ptr response) const
{
	response->setID(id());
	return response;
}

GWResponse::Ptr GWRequest::deriveResponse() const
{
	return deriveGenericResponse(new GWResponse);
}

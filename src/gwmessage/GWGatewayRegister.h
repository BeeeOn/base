#pragma once

#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/IPAddress.h>

#include "gwmessage/GWMessage.h"
#include "model/GatewayID.h"

namespace BeeeOn {

/**
 * @brief Represents message sent by the gateway after connecting to the server.
 * Message is intended to gateway registration on the server.
 *
 * Server confirms successful registration with message GWGatewayAccepted.
 */
class GWGatewayRegister : public GWMessage {
public:
	typedef Poco::SharedPtr<GWGatewayRegister> Ptr;

	GWGatewayRegister();
	GWGatewayRegister(const Poco::JSON::Object::Ptr object);

	void setGatewayID(const GatewayID &gatewayID);
	GatewayID gatewayID() const;

	void setVersion(const std::string &version);
	std::string version() const;

	void setIPAddress(const Poco::Net::IPAddress &ipAddress);
	Poco::Net::IPAddress ipAddress() const;
};

}

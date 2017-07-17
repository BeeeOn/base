#ifndef BEEEON_GW_MESSAGE_SET_VALUE_REQUEST_H
#define BEEEON_GW_MESSAGE_SET_VALUE_REQUEST_H

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWRequest.h"
#include "model/DeviceID.h"
#include "model/ModuleID.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the server to the gateway, intended
 * to change state of an active module on the device.
 *
 * The message contains an identification of the device and its module,
 * a value to be set and a timeout for which the value must be set
 * or the operation end with failed.
 */
class GWSetValueRequest : public GWRequest {
public:
	typedef Poco::SharedPtr<GWSetValueRequest> Ptr;

	GWSetValueRequest();
	GWSetValueRequest(const Poco::JSON::Object::Ptr object);

	void setDeviceID(const DeviceID &deviceID);
	DeviceID deviceID() const;

	void setModuleID(const ModuleID &moduleID);
	ModuleID moduleID() const;

	void setValue(double value);
	double value() const;

	void setTimeout(const Poco::Timespan &timeout);
	Poco::Timespan timeout() const;
};

}

#endif

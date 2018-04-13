#pragma once

#include <set>

#include "gwmessage/GWResponse.h"
#include "model/DeviceID.h"

namespace BeeeOn {

class GWUnpairResponse : public GWResponse {
public:
	GWUnpairResponse();
	GWUnpairResponse(const Poco::JSON::Object::Ptr object);

	void setUnpairedDevices(const std::set<DeviceID> &devices);
	std::set<DeviceID> unpairedDevices() const;
};

}

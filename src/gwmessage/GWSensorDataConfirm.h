#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWMessage.h"

namespace BeeeOn {

/**
 * @brief Represents message sent by the server to the gateway intended
 * to confirm, that the server accepted exported sensor data represented
 * by the GWSensorDataExport.
 *
 * The GWSensorDataConfirm contains unique identifier intended to match
 * a proper GWSensorDataExport.
 */
class GWSensorDataConfirm : public GWMessage {
public:
	typedef Poco::SharedPtr<GWSensorDataConfirm> Ptr;

	GWSensorDataConfirm();
	GWSensorDataConfirm(const Poco::JSON::Object::Ptr object);

};

}

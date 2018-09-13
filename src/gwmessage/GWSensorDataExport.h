#pragma once

#include <vector>

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "gwmessage/GWMessage.h"
#include "gwmessage/GWSensorDataConfirm.h"
#include "model/SensorData.h"

namespace BeeeOn {

/**
 * @brief Represents a message sent by the gateway to the server intended
 * to export measured sensor data.
 *
 * The GWSensorDataExport contains unique identifier intended to match
 * a proper GWSensorDataConfirm (confirmation, that the server accepted
 * exported measured sensor data). After the GWSensorDataExport is sent,
 * the gateway expect a confirmation with the given id.
 */
class GWSensorDataExport : public GWMessage {
public:
	typedef Poco::SharedPtr<GWSensorDataExport> Ptr;

	GWSensorDataExport();
	GWSensorDataExport(const Poco::JSON::Object::Ptr object);

	/**
	 * @brief Creates and returns a corresponding GWSensorDataConfirm message.
	 */
	GWSensorDataConfirm::Ptr confirm() const;

	void setData(const std::vector<SensorData> &data);
	std::vector<SensorData> data() const;
};

}

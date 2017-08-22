#ifndef BEEEON_SENSOR_DATA_H
#define BEEEON_SENSOR_DATA_H

#include <vector>

#include "model/DeviceID.h"
#include "model/ModuleID.h"
#include "model/SensorValue.h"
#include "util/IncompleteTimestamp.h"

namespace BeeeOn {

/*
 * Representation of data coming from a sensor. Every SensorData comes
 * from a single device identified by DeviceID. The SensorData holds
 * a list of measured values.Each of measured values is defined by
 * SensorValue instance.
 */
class SensorData {
public:
	void setDeviceID(const DeviceID &deviceID);
	const DeviceID deviceID() const;

	void setTimestamp(const IncompleteTimestamp &timestamp);
	const IncompleteTimestamp timestamp() const;

	bool isEmpty() const;

	void insertValue(const SensorValue &value);

	std::vector<SensorValue>::iterator begin();
	std::vector<SensorValue>::iterator end();

	std::vector<SensorValue>::const_iterator begin() const;
	std::vector<SensorValue>::const_iterator end() const;

	bool operator !=(const SensorData &data) const;
	bool operator ==(const SensorData &data) const;

private:
	DeviceID m_deviceID;
	IncompleteTimestamp m_timestamp;
	std::vector<SensorValue> m_values;
};

}

#endif

#include "model/SensorData.h"

using namespace std;
using namespace BeeeOn;

void SensorData::setDeviceID(const DeviceID &deviceID)
{
	m_deviceID = deviceID;
}

const DeviceID SensorData::deviceID() const
{
	return m_deviceID;
}

void SensorData::setTimestamp(const IncompleteTimestamp &timestamp)
{
	m_timestamp = timestamp;
}

const IncompleteTimestamp SensorData::timestamp() const
{
	return m_timestamp;
}

bool SensorData::isEmpty() const
{
	return m_values.empty();
}

void SensorData::insertValue(const SensorValue &value)
{
	m_values.push_back(value);
}

vector<SensorValue>::iterator SensorData::begin()
{
	return m_values.begin();
}

vector<SensorValue>::iterator SensorData::end()
{
	return m_values.end();
}

vector<SensorValue>::const_iterator SensorData::begin() const
{
	return m_values.begin();
}

vector<SensorValue>::const_iterator SensorData::end() const
{
	return m_values.end();
}

bool SensorData::operator !=(const SensorData &data) const
{
	return !(*this == data);
}

bool SensorData::operator ==(const SensorData &data) const
{
	return m_deviceID == data.m_deviceID
		&& m_timestamp == data.m_timestamp
		&& m_values == data.m_values;
}


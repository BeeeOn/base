#include <Poco/RegularExpression.h>
#include <Poco/TextConverter.h>
#include <Poco/UTF8Encoding.h>

#include "model/DeviceDescription.h"

using namespace BeeeOn;
using namespace Poco;
using namespace std;

static RegularExpression regex(
	"[^\\p{L}\\p{Nd} \\.:!?()/,\\-_#''$€¥£©®+]",
	RegularExpression::RE_UTF8
);

DeviceDescription::DeviceDescription(
	const DeviceID &deviceID,
	const std::string &vendor,
	const std::string &productName,
	const std::list<ModuleType> &dataTypes,
	Timespan refreshTime):
		m_deviceID(deviceID),
		m_vendor(normalizeName(vendor)),
		m_productName(normalizeName(productName)),
		m_dataTypes(dataTypes),
		m_refreshTime(refreshTime)
{
	if (m_refreshTime < 0)
		m_refreshTime = -1;
	else if (m_refreshTime > 0 && m_refreshTime < Timespan::SECONDS)
		m_refreshTime =  1 * Timespan::SECONDS;
}

DeviceID DeviceDescription::id() const
{
	return m_deviceID;
}

string DeviceDescription::vendor() const
{
	return m_vendor;
}

string DeviceDescription::productName() const
{
	return m_productName;
}

list<ModuleType> DeviceDescription::dataTypes() const
{
	return m_dataTypes;
}

Timespan DeviceDescription::refreshTime() const
{
	return m_refreshTime;
}

string DeviceDescription::toString() const
{
	string result;
	string modules;

	for (auto it = m_dataTypes.begin(); it != m_dataTypes.end(); ++it) {
		modules += it->type().toString();

		const auto &attributes = it->attributes();
		if (!attributes.empty())
			modules += ",";

		for (auto attr = attributes.begin(); attr != attributes.end(); ++attr) {
			modules += attr->toString();

			if (attr != --attributes.end())
				modules += ",";
		}

		if (it != --m_dataTypes.end())
			modules += " ";
	}

	result += m_deviceID.toString() + " ";
	result += m_vendor + " ";
	result += m_productName + " ";
	result += to_string(m_refreshTime.totalSeconds()) + " ";
	result += modules + " ";

	return result;
}

string DeviceDescription::normalizeName(const string& bytes)
{
	const UTF8Encoding utf8;
	TextConverter text(utf8, utf8);
	string result;

	text.convert(bytes, result);
	regex.subst(result, "?", RegularExpression::RE_GLOBAL);

	return result;
}

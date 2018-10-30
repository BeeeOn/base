#include <Poco/RegularExpression.h>
#include <Poco/TextConverter.h>
#include <Poco/UTF8Encoding.h>

#include "model/DeviceDescription.h"

using namespace BeeeOn;
using namespace Poco;
using namespace std;

static const RegularExpression NAME_PATTERN(
	"[^\\p{L}\\p{Nd} \\.:!?()/,\\-_#''$€¥£©®+]",
	RegularExpression::RE_UTF8
);

DeviceDescription::Builder::Builder():
	m_refreshTime(-1)
{
}

DeviceDescription::Builder &DeviceDescription::Builder::id(const DeviceID &id)
{
	m_id = id;
	return *this;
}

DeviceDescription::Builder &DeviceDescription::Builder::type(
		const string &vendor,
		const string &name)
{
	m_vendor = vendor;
	m_product = name;
	return *this;
}

DeviceDescription::Builder &DeviceDescription::Builder::refreshTime(
		const Timespan &time)
{
	m_refreshTime = time;
	return *this;
}

DeviceDescription::Builder &DeviceDescription::Builder::disabledRefreshTime()
{
	m_refreshTime = 0;
	return *this;
}

DeviceDescription::Builder &DeviceDescription::Builder::noRefreshTime()
{
	m_refreshTime = -1;
	return *this;
}

template <typename T>
static T notNull(const Nullable<T> value, const string &label)
{
	if (value.isNull())
		throw InvalidArgumentException(label + " was not set in builder");

	return value.value();
}

DeviceDescription DeviceDescription::Builder::build() const
{
	const DeviceID id = notNull(m_id, "device ID");
	const auto vendor = notNull(m_vendor, "vendor name");
	const auto product = notNull(m_product, "product name");

	return DeviceDescription(id, vendor, product, m_modules, m_refreshTime);
}

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

string DeviceDescription::toPrettyString() const
{
	string summary = m_deviceID.toString() + " : " + m_vendor + " " + m_productName;
	summary.append("\n * RT : " + to_string(m_refreshTime.totalMilliseconds()) + "ms");

	size_t j = 0;
	for (const auto &m : m_dataTypes) {
		summary.append("\n * " + to_string(j++) + " : " + m.type().toString());

		for (const auto &a : m.attributes())
			summary.append("," + a.toString());
	}

	return summary;
}

string DeviceDescription::normalizeName(const string& bytes)
{
	const UTF8Encoding utf8;
	TextConverter text(utf8, utf8);
	string result;

	text.convert(bytes, result);
	NAME_PATTERN.subst(result, "?", RegularExpression::RE_GLOBAL);

	return result;
}

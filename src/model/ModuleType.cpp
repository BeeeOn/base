#include <Poco/RegularExpression.h>
#include <Poco/StringTokenizer.h>

#include "model/ModuleType.h"

using namespace BeeeOn;
using namespace Poco;
using namespace std;

EnumHelper<ModuleType::AttributeEnum::Raw>::ValueMap &ModuleType::AttributeEnum::valueMap()
{
	static EnumHelper<ModuleType::AttributeEnum::Raw>::ValueMap valueMap = {
		{ModuleType::AttributeEnum::TYPE_INNER, "inner"},
		{ModuleType::AttributeEnum::TYPE_MANUAL_ONLY, "manual-only"},
		{ModuleType::AttributeEnum::TYPE_OUTER, "outer"},
	};

	return valueMap;
}

EnumHelper<ModuleType::TypeEnum::Raw>::ValueMap &ModuleType::TypeEnum::valueMap()
{
	static EnumHelper<ModuleType::TypeEnum::Raw>::ValueMap valueMap = {
		{ModuleType::TypeEnum::TYPE_AVAILABILITY, "availability"},
		{ModuleType::TypeEnum::TYPE_BATTERY, "battery"},
		{ModuleType::TypeEnum::TYPE_CO2, "co2"},
		{ModuleType::TypeEnum::TYPE_ENUM, "enum"},
		{ModuleType::TypeEnum::TYPE_FIRE, "fire"},
		{ModuleType::TypeEnum::TYPE_HUMIDITY, "humidity"},
		{ModuleType::TypeEnum::TYPE_LUMINANCE, "luminance"},
		{ModuleType::TypeEnum::TYPE_MOTION, "motion"},
		{ModuleType::TypeEnum::TYPE_NOISE, "noise"},
		{ModuleType::TypeEnum::TYPE_ON_OFF, "on-off"},
		{ModuleType::TypeEnum::TYPE_OPEN_CLOSE, "open-close"},
		{ModuleType::TypeEnum::TYPE_PRESSURE, "pressure"},
		{ModuleType::TypeEnum::TYPE_SECURITY_ALERT, "security-alert"},
		{ModuleType::TypeEnum::TYPE_SHAKE, "shake"},
		{ModuleType::TypeEnum::TYPE_TEMPERATURE, "temperature"},
		{ModuleType::TypeEnum::TYPE_ULTRAVIOLET, "ultraviolet"},
	};

	return valueMap;
}

ModuleType::ModuleType(const ModuleType::Type &type,
		const set<ModuleType::Attribute> &attributes):
	m_type(type),
	m_attributes(attributes)
{
}

ModuleType::ModuleType(const ModuleType::Type &type):
	m_type(type)
{
}

ModuleType::ModuleType(const ModuleType::Type &type, const CustomTypeID &customID):
	m_type(type),
	m_customID(customID)
{
	switch (m_type) {
	case ModuleType::Type::TYPE_ENUM:
		break;
	default:
		throw InvalidArgumentException(
			"module type " + m_type.toString() + " does not support type ID");
	}
}

ModuleType::ModuleType(const ModuleType::Type &type, const CustomTypeID &customID,
		const std::set<Attribute> &attributes):
	m_type(type),
	m_attributes(attributes),
	m_customID(customID)
{
	switch (m_type) {
	case ModuleType::Type::TYPE_ENUM:
		break;
	default:
		throw InvalidArgumentException(
			"module type " + m_type.toString() + " does not support type ID");
	}
}

void ModuleType::setType(const ModuleType::Type &type)
{
	m_type = type;
}

ModuleType::Type ModuleType::type() const
{
	return m_type;
}

void ModuleType::setAttributes(const set<ModuleType::Attribute> &attributes)
{
	m_attributes = attributes;
}

set<ModuleType::Attribute> ModuleType::attributes() const
{
	return m_attributes;
}

void ModuleType::setCustomTypeID(CustomTypeID id)
{
	switch (m_type) {
	case ModuleType::Type::TYPE_ENUM:
		break;
	default:
		throw IllegalStateException(
			"module type " + m_type.toString() + " does not support type ID");
	}

	m_customID = id;
}

CustomTypeID ModuleType::customTypeID() const
{
	return m_customID;
}

ModuleType ModuleType::parse(string input)
{
	set<ModuleType::Attribute> attributes;

	const StringTokenizer tokens(input, ",",
		StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);

	for (unsigned int k = 1; k < tokens.count(); k++) {
		ModuleType::Attribute current =
			ModuleType::Attribute::parse(tokens[k]);

		auto it = attributes.find(current);
		if (it != attributes.end()) {
			throw InvalidArgumentException(
				"duplicate attribute '" + input + "'");
		}

		attributes.insert(current);
	}

	RegularExpression::MatchVec matches;
	RegularExpression re("(enum):(.+)");

	string typeName;
	string idValue;

	if (re.match(tokens[0], 0, matches) == 3) {
		typeName = tokens[0].substr(matches[1].offset, matches[1].length);
		idValue = tokens[0].substr(matches[2].offset, matches[2].length);
	}
	else {
		typeName = tokens[0];
	}

	const ModuleType::Type type = ModuleType::Type::parse(typeName);

	switch (type) {
	case ModuleType::Type::TYPE_ENUM:
		if (idValue.empty())
			throw InvalidArgumentException("missing ID for type " + type.toString());
		break;
	default:
		return ModuleType(type, attributes);
	}

	const auto id = CustomTypeID::parse(idValue);
	return ModuleType(type, id, attributes);
}

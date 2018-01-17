#include <Poco/RegularExpression.h>
#include <Poco/StringTokenizer.h>

#include "model/ModuleType.h"

using namespace BeeeOn;
using namespace Poco;
using namespace std;

EnumHelper<ModuleType::AttributeEnum::Raw>::ValueMap &ModuleType::AttributeEnum::valueMap()
{
	static EnumHelper<ModuleType::AttributeEnum::Raw>::ValueMap valueMap = {
		{ModuleType::AttributeEnum::ATTR_INNER, "inner"},
		{ModuleType::AttributeEnum::ATTR_MANUAL_ONLY, "manual-only"},
		{ModuleType::AttributeEnum::ATTR_OUTER, "outer"},
		{ModuleType::AttributeEnum::ATTR_CONTROLLABLE, "controllable"},
	};

	return valueMap;
}

EnumHelper<ModuleType::TypeEnum::Raw>::ValueMap &ModuleType::TypeEnum::valueMap()
{
	static EnumHelper<ModuleType::TypeEnum::Raw>::ValueMap valueMap = {
		{ModuleType::TypeEnum::TYPE_AVAILABILITY, "availability"},
		{ModuleType::TypeEnum::TYPE_BATTERY, "battery"},
		{ModuleType::TypeEnum::TYPE_BITMAP, "bitmap"},
		{ModuleType::TypeEnum::TYPE_BRIGHTNESS, "brightness"},
		{ModuleType::TypeEnum::TYPE_CO2, "co2"},
		{ModuleType::TypeEnum::TYPE_ENUM, "enum"},
		{ModuleType::TypeEnum::TYPE_FIRE, "fire"},
		{ModuleType::TypeEnum::TYPE_HUMIDITY, "humidity"},
		{ModuleType::TypeEnum::TYPE_LUMINANCE, "luminance"},
		{ModuleType::TypeEnum::TYPE_MOTION, "motion"},
		{ModuleType::TypeEnum::TYPE_NOISE, "noise"},
		{ModuleType::TypeEnum::TYPE_ON_OFF, "on-off"},
		{ModuleType::TypeEnum::TYPE_OPEN_CLOSE, "open-close"},
		{ModuleType::TypeEnum::TYPE_PERFORMANCE, "performance"},
		{ModuleType::TypeEnum::TYPE_PRESSURE, "pressure"},
		{ModuleType::TypeEnum::TYPE_SECURITY_ALERT, "security-alert"},
		{ModuleType::TypeEnum::TYPE_SHAKE, "shake"},
		{ModuleType::TypeEnum::TYPE_TEMPERATURE, "temperature"},
		{ModuleType::TypeEnum::TYPE_ULTRAVIOLET, "ultraviolet"},
		{ModuleType::TypeEnum::TYPE_POWER, "power"},
		{ModuleType::TypeEnum::TYPE_VOLTAGE, "voltage"},
		{ModuleType::TypeEnum::TYPE_CURRENT, "current"},
	};

	return valueMap;
}

ModuleType::ModuleType(const ModuleType::Type &type,
		const set<ModuleType::Attribute> &attributes):
	m_type(type),
	m_attributes(attributes)
{
	assureValidAttributes(attributes);
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
	case ModuleType::Type::TYPE_BITMAP:
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
	assureValidAttributes(attributes);

	switch (m_type) {
	case ModuleType::Type::TYPE_ENUM:
		break;
	case ModuleType::Type::TYPE_BITMAP:
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
	assureValidAttributes(attributes);
	m_attributes = attributes;
}

set<ModuleType::Attribute> ModuleType::attributes() const
{
	return m_attributes;
}

bool ModuleType::hasCombination(
	const set<Attribute> &attributes,
	const set<Attribute> &check)
{
	for (const auto &one : check) {
		if (attributes.find(one) == attributes.end())
			return false;
	}

	return true;
}

void ModuleType::assureValidAttributes(const set<Attribute> &attributes)
{
	if (hasCombination(attributes,
			{Attribute::ATTR_INNER, Attribute::ATTR_OUTER})) {
		throw InvalidArgumentException(
			"inner and outer cannot be set at once");
	}

	if (attributes.find(Attribute::ATTR_MANUAL_ONLY) != attributes.end()) {
		if (attributes.find(Attribute::ATTR_CONTROLLABLE) == attributes.end()) {
			throw InvalidArgumentException(
				"manual-only attribute requires attribute controllable");
		}
	}
}

bool ModuleType::isControllable() const
{
	return m_attributes.find(Attribute::ATTR_CONTROLLABLE) != m_attributes.end();
}

void ModuleType::setCustomTypeID(CustomTypeID id)
{
	switch (m_type) {
	case ModuleType::Type::TYPE_ENUM:
		break;
	case ModuleType::Type::TYPE_BITMAP:
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

	assureValidAttributes(attributes);

	RegularExpression::MatchVec matches;
	RegularExpression re("(enum|bitmap):(.+)");

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
	case ModuleType::Type::TYPE_BITMAP:
		if (idValue.empty())
			throw InvalidArgumentException("missing ID for type " + type.toString());
		break;
	default:
		return ModuleType(type, attributes);
	}

	const auto id = CustomTypeID::parse(idValue);
	return ModuleType(type, id, attributes);
}

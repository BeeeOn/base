#ifndef BEEEON_MODULE_TYPE_H
#define BEEEON_MODULE_TYPE_H

#include <set>

#include "model/CustomTypeID.h"
#include "util/Enum.h"

namespace BeeeOn {

/**
 * Representation of value type that device can send. Each value
 * consists of two parts: Type and Attribute.
 *
 * The attribute is optional. The type is high level data type
 * and the attribute is additional information, for example
 * location (inner, outer).
 */
class ModuleType {
public:
	struct AttributeEnum {
		enum Raw {
			/**
			 * The sensor/actuator is located inside (a building).
			 */
			TYPE_INNER,
			/**
			 * The sensor enables to set (to modify) value only from sensor.
			 */
			TYPE_MANUAL_ONLY,
			/**
			 * The sensor/actuator is located outside.
			 */
			TYPE_OUTER,
		};

		static EnumHelper<Raw>::ValueMap &valueMap();
	};

	struct TypeEnum {
		enum Raw {
			/**
			 * Value 0: unreachable
			 * Value 1: reachable
			 */
			TYPE_AVAILABILITY,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_BATTERY,
			/**
			 * It defines specific bitmap module
			 * type which is stored in m_customID.
			 */
			TYPE_BITMAP,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_BRIGHTNESS,
			/**
			 * Min: 0
			 * Unit: one part per million (ppm)
			 */
			TYPE_CO2,
			/**
			 * It defines specific enum module
			 * type which is stored in m_customID.
			 */
			TYPE_ENUM,
			/**
			 * Value 0: no fire detected
			 * Value 1: fire detected
			 */
			TYPE_FIRE,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_HUMIDITY,
			/**
			 * Range: 0..100000
			 * Unit: lux
			 */
			TYPE_LUMINANCE,
			/**
			 * Value 0: no motion detected
			 * Value 1: motion detected
			 */
			TYPE_MOTION,
			/**
			 * Unit: dB
			 */
			TYPE_NOISE,
			/**
			 * Value 0: open
			 * Value 1: closed
			 */
			TYPE_OPEN_CLOSE,
			/**
			 * Value 0: off
			 * Value 1: on
			 */
			TYPE_ON_OFF,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_PERFORMANCE,
			/**
			 * Unit: hPa
			 */
			TYPE_PRESSURE,
			/**
			 * Range: 0..100
			 * Unit: %
			 */
			TYPE_RSSI,
			/**
			 * Value 0: no security alert
			 * Value 1: security alert is active
			 *
			 */
			TYPE_SECURITY_ALERT,
			/**
			 * Value 0: no shake detect
			 * Value 1: shake detect
			 */
			TYPE_SHAKE,
			/**
			 * Unit: °C
			 */
			TYPE_TEMPERATURE,
			/**
			 * Ultraviolet light
			 * Unit: UV index
			 * Range: 0..11
			 */
			TYPE_ULTRAVIOLET,
		};

		static EnumHelper<Raw>::ValueMap &valueMap();
	};

	typedef Enum<TypeEnum> Type;
	typedef Enum<AttributeEnum> Attribute;

	ModuleType(const Type &type);
	ModuleType(const Type &type, const std::set<Attribute> &attributes);
	ModuleType(const Type &type, const CustomTypeID &customID);
	ModuleType(const Type &type, const CustomTypeID &customID, const std::set<Attribute> &attributes);

	void setType(const Type &type);
	Type type() const;

	void setAttributes(const std::set<Attribute> &attributes);
	std::set<Attribute> attributes() const;

	void setCustomTypeID(CustomTypeID id);
	CustomTypeID customTypeID() const;

	static ModuleType parse(std::string input);

private:
	Type m_type;
	std::set<Attribute> m_attributes;
	CustomTypeID m_customID;
};

}

#endif

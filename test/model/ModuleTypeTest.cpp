#include <set>
#include <cppunit/extensions/HelperMacros.h>
#include "cppunit/BetterAssert.h"

#include "model/ModuleType.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class ModuleTypeTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(ModuleTypeTest);
	CPPUNIT_TEST(testParse);
	CPPUNIT_TEST(testParseInvalidEnum);
	CPPUNIT_TEST(testParseInvalidBitmap);
	CPPUNIT_TEST(testInvalidArgumentType);
	CPPUNIT_TEST(testInvalidArgumentAttribute);
	CPPUNIT_TEST(testInvalidAttributeDuplication);
	CPPUNIT_TEST(testAttributesConflicts);
	CPPUNIT_TEST(testUnitIsValid);
	CPPUNIT_TEST_SUITE_END();
public:
	void testParse();
	void testParseInvalidEnum();
	void testParseInvalidBitmap();
	void testInvalidArgumentType();
	void testInvalidArgumentAttribute();
	void testInvalidAttributeDuplication();
	void testAttributesConflicts();
	void testUnitIsValid();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ModuleTypeTest);

void ModuleTypeTest::testParse()
{
	const ModuleType &moduleType = ModuleType::parse("temperature,inner");

	CPPUNIT_ASSERT_EQUAL("temperature", moduleType.type().toString());

	for (auto &item : moduleType.attributes())
		CPPUNIT_ASSERT_EQUAL("inner", item.toString());

	CPPUNIT_ASSERT_EQUAL(1, moduleType.attributes().size());

	const ModuleType &customType1 = ModuleType::parse("enum:custom1,inner");

	CPPUNIT_ASSERT_EQUAL("enum", customType1.type().toString());
	CPPUNIT_ASSERT_EQUAL("custom1", customType1.customTypeID().toString());

	for (auto &item : customType1.attributes())
		CPPUNIT_ASSERT_EQUAL("inner", item.toString());

	CPPUNIT_ASSERT_EQUAL(1, customType1.attributes().size());

	const ModuleType &customType2 = ModuleType::parse("bitmap:custom2,inner");

	CPPUNIT_ASSERT_EQUAL("bitmap", customType2.type().toString());
	CPPUNIT_ASSERT_EQUAL("custom2", customType2.customTypeID().toString());

	for (auto &item : customType2.attributes())
		CPPUNIT_ASSERT_EQUAL("inner", item.toString());

	CPPUNIT_ASSERT_EQUAL(1, customType2.attributes().size());

	const ModuleType &customType3 = ModuleType::parse("humidity,controllable");

	CPPUNIT_ASSERT_EQUAL("humidity", customType3.type().toString());
	CPPUNIT_ASSERT(customType3.isControllable());

	for (auto &item : customType3.attributes())
		CPPUNIT_ASSERT_EQUAL("controllable", item.toString());

	CPPUNIT_ASSERT_EQUAL(1, customType3.attributes().size());
}

void ModuleTypeTest::testParseInvalidEnum()
{
	CPPUNIT_ASSERT_THROW(
		ModuleType::parse("enum"),
		InvalidArgumentException
	);

	CPPUNIT_ASSERT_THROW(
		ModuleType::parse("enum:"),
		InvalidArgumentException
	);

	CPPUNIT_ASSERT_THROW(
		ModuleType::parse("enum,inner"),
		InvalidArgumentException
	);

	CPPUNIT_ASSERT_THROW(
		ModuleType::parse("enum:,inner"),
		InvalidArgumentException
	);
}

void ModuleTypeTest::testParseInvalidBitmap()
{
	CPPUNIT_ASSERT_THROW(
		ModuleType::parse("bitmap"),
		InvalidArgumentException
	);

	CPPUNIT_ASSERT_THROW(
		ModuleType::parse("bitmap:"),
		InvalidArgumentException
	);

	CPPUNIT_ASSERT_THROW(
		ModuleType::parse("bitmap,inner"),
		InvalidArgumentException
	);

	CPPUNIT_ASSERT_THROW(
		ModuleType::parse("bitmap:,inner"),
		InvalidArgumentException
	);
}

void ModuleTypeTest::testInvalidArgumentType()
{
	CPPUNIT_ASSERT_THROW(ModuleType::parse("undefined-type,inner"), InvalidArgumentException);
}

void ModuleTypeTest::testInvalidArgumentAttribute()
{
	CPPUNIT_ASSERT_THROW(ModuleType::parse("temperature,undefined-attribute"), InvalidArgumentException);
}

void ModuleTypeTest::testInvalidAttributeDuplication()
{
	CPPUNIT_ASSERT_THROW(ModuleType::parse("humidity,inner,inner"), InvalidArgumentException);
}

void ModuleTypeTest::testAttributesConflicts()
{
	CPPUNIT_ASSERT_THROW(ModuleType::parse("temperature,inner,outer"), InvalidArgumentException);
	CPPUNIT_ASSERT_THROW(ModuleType::parse("temperature,manual-only"), InvalidArgumentException);
	CPPUNIT_ASSERT_NO_THROW(
		ModuleType::parse("temperature,manual-only,controllable")
	);
}

void ModuleTypeTest::testUnitIsValid()
{
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::NONE).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::NONE).isValid(-1));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::NONE).isValid(1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::NONE).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::BINARY).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::BINARY).isValid(1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::BINARY).isValid(2));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::BINARY).isValid(-1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::BINARY).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::PERCENT).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::PERCENT).isValid(100));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::PERCENT).isValid(101));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::PERCENT).isValid(-1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::PERCENT).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::PPM).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::PPM).isValid(100));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::PPM).isValid(100000));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::PPM).isValid(-1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::PPM).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::LUX).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::LUX).isValid(100000));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::LUX).isValid(100001));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::LUX).isValid(-1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::LUX).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::DECIBEL).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::DECIBEL).isValid(-1));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::DECIBEL).isValid(1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::DECIBEL).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::HECTOPASCAL).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::HECTOPASCAL).isValid(-1));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::HECTOPASCAL).isValid(1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::HECTOPASCAL).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::CELSIUS).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::CELSIUS).isValid(1000));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::CELSIUS).isValid(-273.15));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::CELSIUS).isValid(-273.16));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::CELSIUS).isValid(-274));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::CELSIUS).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::UVINDEX).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::UVINDEX).isValid(11));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::UVINDEX).isValid(12));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::UVINDEX).isValid(-1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::UVINDEX).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::WATT).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::WATT).isValid(-1));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::WATT).isValid(1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::WATT).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::VOLT).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::VOLT).isValid(-1));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::VOLT).isValid(1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::VOLT).isValid(0.0 / 0.0));

	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::AMPERE).isValid(0));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::AMPERE).isValid(-1));
	CPPUNIT_ASSERT(ModuleType::Unit(ModuleType::Unit::AMPERE).isValid(1));
	CPPUNIT_ASSERT(!ModuleType::Unit(ModuleType::Unit::AMPERE).isValid(0.0 / 0.0));
}

}

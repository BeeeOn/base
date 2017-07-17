#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/JSON/JSONException.h>

#include "cppunit/BetterAssert.h"
#include "gwmessage/GWMessage.h"
#include "util/JsonUtil.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;

namespace BeeeOn {

class GWMessageTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(GWMessageTest);
	CPPUNIT_TEST(testParseEmpty);
	CPPUNIT_TEST(testParseEmptyObject);
	CPPUNIT_TEST(testParseUnknownType);
	CPPUNIT_TEST_SUITE_END();
public:
	void testParseEmpty();
	void testParseEmptyObject();
	void testParseUnknownType();
protected:
	string jsonReformat(const string &json);
};

CPPUNIT_TEST_SUITE_REGISTRATION(GWMessageTest);

string GWMessageTest::jsonReformat(const string &json)
{
	return Dynamic::Var::toString(JsonUtil::parse(json));
}

void GWMessageTest::testParseEmpty()
{
	CPPUNIT_ASSERT_THROW(
		GWMessage::fromJSON(""),
		JSONException
	);
}

void GWMessageTest::testParseEmptyObject()
{
	CPPUNIT_ASSERT_THROW(
		GWMessage::fromJSON("{}"),
		InvalidAccessException
	);
}

void GWMessageTest::testParseUnknownType()
{
	CPPUNIT_ASSERT_THROW(
		GWMessage::fromJSON(R"({"message_type" : "unknown_type"})"),
		InvalidArgumentException
	);
}

}

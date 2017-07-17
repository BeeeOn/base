#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/JSON/JSONException.h>

#include "cppunit/BetterAssert.h"
#include "gwmessage/GWMessage.h"
#include "gwmessage/GWGatewayRegister.h"
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
	CPPUNIT_TEST(testParseGatewayRegister);
	CPPUNIT_TEST(testCreateGatewayRegister);
	CPPUNIT_TEST_SUITE_END();
public:
	void testParseEmpty();
	void testParseEmptyObject();
	void testParseUnknownType();
	void testParseGatewayRegister();
	void testCreateGatewayRegister();
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

void GWMessageTest::testParseGatewayRegister()
{
	GWMessage::Ptr message = GWMessage::fromJSON(
	R"({
			"message_type" : "gateway_register",
			"gateway_id" : "1863705252509532",
			"version" : "v1.0",
			"ip_address" : "192.168.1.1"
	})");

	CPPUNIT_ASSERT_EQUAL(GWMessageType::GATEWAY_REGISTER, message->type().raw());
	CPPUNIT_ASSERT(!message.cast<GWGatewayRegister>().isNull());

	GWGatewayRegister::Ptr registerMessage = message.cast<GWGatewayRegister>();
	CPPUNIT_ASSERT(registerMessage->gatewayID().toString() == "1863705252509532");
	CPPUNIT_ASSERT(registerMessage->version() == "v1.0");
	CPPUNIT_ASSERT(registerMessage->ipAddress().toString() == "192.168.1.1");
}

void GWMessageTest::testCreateGatewayRegister()
{
	GWGatewayRegister::Ptr message(new GWGatewayRegister);
	message->setGatewayID(GatewayID::parse("1863705252509532"));
	message->setVersion("v1.0");
	message->setIPAddress(Net::IPAddress("192.168.1.1"));

	CPPUNIT_ASSERT_EQUAL(
		jsonReformat(R"({
			"message_type" : "gateway_register",
			"gateway_id" : "1863705252509532",
			"version" : "v1.0",
			"ip_address" : "192.168.1.1"
		})"),
		message->toString()
	);
}

}

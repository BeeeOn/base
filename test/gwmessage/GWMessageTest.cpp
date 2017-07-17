#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/JSON/JSONException.h>

#include "cppunit/BetterAssert.h"
#include "gwmessage/GWMessage.h"
#include "gwmessage/GWGatewayRegister.h"
#include "gwmessage/GWGatewayAccepted.h"
#include "gwmessage/GWRequest.h"
#include "gwmessage/GWResponse.h"
#include "gwmessage/GWAck.h"
#include "gwmessage/GWResponseWithAck.h"
#include "model/GlobalID.h"
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
	CPPUNIT_TEST(testParseGatewayAccepted);
	CPPUNIT_TEST(testCreateGatewayAccepted);
	CPPUNIT_TEST(testDeriveResponse);
	CPPUNIT_TEST(testGetAckFromResponse);
	CPPUNIT_TEST_SUITE_END();
public:
	void testParseEmpty();
	void testParseEmptyObject();
	void testParseUnknownType();
	void testParseGatewayRegister();
	void testCreateGatewayRegister();
	void testParseGatewayAccepted();
	void testCreateGatewayAccepted();
	void testDeriveResponse();
	void testGetAckFromResponse();
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

void GWMessageTest::testParseGatewayAccepted()
{
	GWMessage::Ptr message = GWMessage::fromJSON(
	R"({
			"message_type" : "gateway_accepted"
	})");

	CPPUNIT_ASSERT_EQUAL(GWMessageType::GATEWAY_ACCEPTED, message->type().raw());
	CPPUNIT_ASSERT(!message.cast<GWGatewayAccepted>().isNull());
}

void GWMessageTest::testCreateGatewayAccepted()
{
	GWGatewayAccepted::Ptr message(new GWGatewayAccepted);

	CPPUNIT_ASSERT_EQUAL(
		jsonReformat(R"({
			"message_type" : "gateway_accepted"
		})"),
		message->toString()
	);
}

class TestingResponse : public GWResponse {
public:
	typedef SharedPtr<TestingResponse> Ptr;

	void setText(const string &text)
	{
		json()->set("text", text);
	}

	string text() const
	{
		return json()->getValue<string>("text");
	}
};

class TestingRequest : public GWRequest {
public:
	TestingRequest():
		GWRequest(GWMessageType::GATEWAY_REGISTER)
	{
	}

	GWResponse::Ptr deriveResponse() const override
	{
		return deriveGenericResponse(new TestingResponse);
	}
};

void GWMessageTest::testDeriveResponse()
{
	TestingRequest request;
	request.setID(GlobalID::parse("4a41d041-eb1e-4e9c-9528-1bbe74f54d59"));

	GWResponse::Ptr resp1 = request.derive();
	CPPUNIT_ASSERT(GWMessageType::GENERIC_RESPONSE == resp1->type());
	CPPUNIT_ASSERT_EQUAL("4a41d041-eb1e-4e9c-9528-1bbe74f54d59", resp1->id().toString());

	TestingResponse::Ptr resp2 = request.derive<TestingResponse>(
		[](TestingResponse::Ptr r) {
			r->setText("test text");
		}
	);

	CPPUNIT_ASSERT(GWMessageType::GENERIC_RESPONSE == resp1->type());
	CPPUNIT_ASSERT_EQUAL("test text", resp2->text());
	CPPUNIT_ASSERT_EQUAL("4a41d041-eb1e-4e9c-9528-1bbe74f54d59", resp2->id().toString());
}

void GWMessageTest::testGetAckFromResponse()
{
	GWResponseWithAck::Ptr resp(new GWResponseWithAck);
	resp->setID(GlobalID::parse("4a41d041-eb1e-4e9c-9528-1bbe74f54d59"));
	resp->setStatus(GWResponse::SUCCESS);

	GWAck::Ptr ack = resp->ack();

	CPPUNIT_ASSERT(!ack.isNull());
	CPPUNIT_ASSERT_EQUAL(GWMessageType::GENERIC_ACK, ack->type().raw());
	CPPUNIT_ASSERT_EQUAL("4a41d041-eb1e-4e9c-9528-1bbe74f54d59", ack->id().toString());
	CPPUNIT_ASSERT_EQUAL(GWResponse::SUCCESS, ack->status());
}

}

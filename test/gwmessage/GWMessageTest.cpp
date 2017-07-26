#include <algorithm>

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
#include "gwmessage/GWSensorDataConfirm.h"
#include "gwmessage/GWSensorDataExport.h"
#include "gwmessage/GWNewDeviceRequest.h"
#include "gwmessage/GWLastValueResponse.h"
#include "gwmessage/GWLastValueRequest.h"
#include "gwmessage/GWDeviceListResponse.h"
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
	CPPUNIT_TEST(testParseSensorDataConfirm);
	CPPUNIT_TEST(testCreateSensorDataConfirm);
	CPPUNIT_TEST(testParseSensorDataExport);
	CPPUNIT_TEST(testCreateSensorDataExport);
	CPPUNIT_TEST(testGetConfirmFromSensorDataExport);
	CPPUNIT_TEST(testParseNewDevice);
	CPPUNIT_TEST(testCreateNewDevice);
	CPPUNIT_TEST(testParseLastValue);
	CPPUNIT_TEST(testCreateLastValue);
	CPPUNIT_TEST(testParseDeviceList);
	CPPUNIT_TEST(testCreateDeviceList);
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
	void testParseSensorDataConfirm();
	void testCreateSensorDataConfirm();
	void testParseSensorDataExport();
	void testCreateSensorDataExport();
	void testGetConfirmFromSensorDataExport();
	void testParseNewDevice();
	void testCreateNewDevice();
	void testParseLastValue();
	void testCreateLastValue();
	void testParseDeviceList();
	void testCreateDeviceList();
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

void GWMessageTest::testParseSensorDataConfirm()
{
	GWMessage::Ptr message = GWMessage::fromJSON(
	R"({
			"message_type" : "sensor_data_confirm",
			"id" : "4a41d041-eb1e-4e9c-9528-1bbe74f54d59"
	})");

	CPPUNIT_ASSERT_EQUAL(GWMessageType::SENSOR_DATA_CONFIRM, message->type().raw());
	CPPUNIT_ASSERT(!message.cast<GWSensorDataConfirm>().isNull());

	GWSensorDataConfirm::Ptr confirm = message.cast<GWSensorDataConfirm>();
	CPPUNIT_ASSERT_EQUAL("4a41d041-eb1e-4e9c-9528-1bbe74f54d59", confirm->id().toString() );
}

void GWMessageTest::testCreateSensorDataConfirm()
{
	GWSensorDataConfirm::Ptr message(new GWSensorDataConfirm);
	message->setID(GlobalID::parse("4a41d041-eb1e-4e9c-9528-1bbe74f54d59"));

	CPPUNIT_ASSERT_EQUAL(
		jsonReformat(R"({
			"message_type" : "sensor_data_confirm",
			"id" : "4a41d041-eb1e-4e9c-9528-1bbe74f54d59"
		})"),
		message->toString()
	);
}

void GWMessageTest::testParseSensorDataExport()
{
	GWMessage::Ptr message = GWMessage::fromJSON(
	R"({
			"message_type" : "sensor_data_export",
			"id" : "4a41d041-eb1e-4e9c-9528-1bbe74f54d59",
			"data" : [
				{
					"device_id" : "0xa123123412341234",
					"timestamp" : 1500334250150150,
					"values": [
						{
							"module_id" : "0",
							"valid" : true,
							"value" : 3.5
						}
					]
				},
				{
					"device_id" : "0xa123444455556666",
					"timestamp" : 1111111111111111,
					"values": [
						{
							"module_id" : "0",
							"valid" : true,
							"value" : 3.5
						},
						{
							"module_id": "1",
							"valid": false
						}
					]
				}
			]
	})");

	CPPUNIT_ASSERT_EQUAL(GWMessageType::SENSOR_DATA_EXPORT, message->type().raw());
	CPPUNIT_ASSERT(!message.cast<GWSensorDataExport>().isNull());

	GWSensorDataExport::Ptr dataExport = message.cast<GWSensorDataExport>();
	CPPUNIT_ASSERT_EQUAL("4a41d041-eb1e-4e9c-9528-1bbe74f54d59", dataExport->id().toString() );

	vector<SensorData> data = dataExport->data();
	CPPUNIT_ASSERT(data.size() == 2);

	CPPUNIT_ASSERT_EQUAL("0xa123123412341234", data[0].deviceID().toString());
	CPPUNIT_ASSERT_EQUAL(1500334250150150, data[0].timestamp().value().epochMicroseconds());
	CPPUNIT_ASSERT_EQUAL(0, data[0].begin()->moduleID().value());
	CPPUNIT_ASSERT_EQUAL(3.5, data[0].begin()->value());
	CPPUNIT_ASSERT(data[0].begin()->isValid());

	CPPUNIT_ASSERT_EQUAL("0xa123444455556666", data[1].deviceID().toString());
	CPPUNIT_ASSERT_EQUAL(1111111111111111, data[1].timestamp().value().epochMicroseconds());
	CPPUNIT_ASSERT_EQUAL(0, data[1].begin()->moduleID().value());
	CPPUNIT_ASSERT_EQUAL(3.5, data[1].begin()->value());
	CPPUNIT_ASSERT(data[1].begin()->isValid());
	CPPUNIT_ASSERT_EQUAL(1, (data[1].begin() + 1)->moduleID().value());
	CPPUNIT_ASSERT(!(data[1].begin() + 1)->isValid());
}

void GWMessageTest::testCreateSensorDataExport()
{
	GWSensorDataExport::Ptr message(new GWSensorDataExport);
	message->setID(GlobalID::parse("4a41d041-eb1e-4e9c-9528-1bbe74f54d59"));

	SensorValue value1;
	value1.setModuleID(0);
	value1.setValue(3.5);
	value1.setValid(true);

	SensorValue value2;
	value2.setModuleID(1);
	value2.setValue(NAN);
	value2.setValid(false);

	SensorData data1;
	data1.setDeviceID(DeviceID::parse("0xa123123412341234"));
	data1.setTimestamp(Timestamp(1500334250150150));
	data1.insertValue(value1);

	SensorData data2;
	data2.setDeviceID(DeviceID::parse("0xa123444455556666"));
	data2.setTimestamp(Timestamp(1111111111111111));
	data2.insertValue(value1);
	data2.insertValue(value2);

	vector<SensorData> dataVector;
	dataVector.push_back(data1);
	dataVector.push_back(data2);

	message->setData(dataVector);

	CPPUNIT_ASSERT_EQUAL(
		jsonReformat(R"({
			"message_type" : "sensor_data_export",
			"id" : "4a41d041-eb1e-4e9c-9528-1bbe74f54d59",
			"data" : [
				{
					"device_id" : "0xa123123412341234",
					"timestamp" : 1500334250150150,
					"values": [
						{
							"module_id" : "0",
							"valid" : true,
							"value" : 3.5
						}
					]
				},
				{
					"device_id" : "0xa123444455556666",
					"timestamp" : 1111111111111111,
					"values": [
						{
							"module_id" : "0",
							"valid" : true,
							"value" : 3.5
						},
						{
							"module_id": "1",
							"valid": false
						}
					]
				}
			]
		})"),
		message->toString()
	);
}

void GWMessageTest::testGetConfirmFromSensorDataExport()
{
	GWSensorDataExport::Ptr message(new GWSensorDataExport);
	message->setID(GlobalID::parse("4a41d041-eb1e-4e9c-9528-1bbe74f54d59"));

	GWSensorDataConfirm::Ptr confirm = message->confirm();

	CPPUNIT_ASSERT(!confirm.isNull());
	CPPUNIT_ASSERT_EQUAL(GWMessageType::SENSOR_DATA_CONFIRM, confirm->type().raw());
	CPPUNIT_ASSERT_EQUAL("4a41d041-eb1e-4e9c-9528-1bbe74f54d59", confirm->id().toString());
}

void GWMessageTest::testParseNewDevice()
{
	GWMessage::Ptr message = GWMessage::fromJSON(
	R"({
			"message_type" : "new_device_request",
			"id" : "4a41d041-eb1e-4e9c-9528-1bbe74f54d59",
			"device_id" : "0xfe01020304050607",
			"vendor" : "Good Company",
			"product_name" : "Nice Product",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "humidity",
					"attributes" : [
						{"attribute" : "inner"}
					]
				},
				{
					"type" : "pressure",
					"attributes" : [
						{"attribute" : "outer"},
						{"attribute" : "manual-only"}
					]
				}
			]
	})");

	CPPUNIT_ASSERT_EQUAL(GWMessageType::NEW_DEVICE_REQUEST, message->type().raw());
	CPPUNIT_ASSERT(!message.cast<GWNewDeviceRequest>().isNull());

	GWNewDeviceRequest::Ptr request = message.cast<GWNewDeviceRequest>();
	CPPUNIT_ASSERT_EQUAL("4a41d041-eb1e-4e9c-9528-1bbe74f54d59", request->id().toString());
	CPPUNIT_ASSERT_EQUAL("0xfe01020304050607", request->deviceID().toString());
	CPPUNIT_ASSERT_EQUAL("Good Company", request->vendor());
	CPPUNIT_ASSERT_EQUAL("Nice Product", request->productName());
	CPPUNIT_ASSERT(Timespan(30, 0) == request->refreshTime());

	const list<ModuleType> &types = request->moduleTypes();

	CPPUNIT_ASSERT_EQUAL("humidity", types.begin()->type().toString());
	CPPUNIT_ASSERT_EQUAL("inner", types.begin()->attributes().begin()->toString());
	CPPUNIT_ASSERT_EQUAL("pressure", types.rbegin()->type().toString());

	const set<ModuleType::Attribute> &attributes = types.rbegin()->attributes();
	CPPUNIT_ASSERT(attributes.size() == 2);
	CPPUNIT_ASSERT(attributes.find(ModuleType::Attribute::TYPE_OUTER) != attributes.end());
	CPPUNIT_ASSERT(attributes.find(ModuleType::Attribute::TYPE_MANUAL_ONLY) != attributes.end());
}

void GWMessageTest::testCreateNewDevice()
{
	GWNewDeviceRequest::Ptr request(new GWNewDeviceRequest);
	request->setID(GlobalID::parse("4a41d041-eb1e-4e9c-9528-1bbe74f54d59"));
	request->setDeviceID(DeviceID::parse("0xfe01020304050607"));
	request->setVendor("Good Company");
	request->setProductName("Nice Product");
	request->setRefreshTime(Timespan(30, 0));

	set<ModuleType::Attribute> attributes1;
	attributes1.emplace(ModuleType::Attribute::TYPE_INNER);
	ModuleType type1(ModuleType::Type::fromRaw(
		ModuleType::Type::TYPE_HUMIDITY), attributes1);

	set<ModuleType::Attribute> attributes2;
	attributes2.emplace(ModuleType::Attribute::TYPE_MANUAL_ONLY);
	attributes2.emplace(ModuleType::Attribute::TYPE_OUTER);
	ModuleType type2(ModuleType::Type::fromRaw(
		ModuleType::Type::TYPE_PRESSURE), attributes2);

	list<ModuleType> types;
	types.push_back(type1);
	types.push_back(type2);

	request->setModuleTypes(types);

	CPPUNIT_ASSERT_EQUAL(
		jsonReformat(R"({
			"message_type" : "new_device_request",
			"id" : "4a41d041-eb1e-4e9c-9528-1bbe74f54d59",
			"device_id" : "0xfe01020304050607",
			"vendor" : "Good Company",
			"product_name" : "Nice Product",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "humidity",
					"attributes" : [
						{"attribute" : "inner"}
					]
				},
				{
					"type" : "pressure",
					"attributes" : [
						{"attribute" : "manual-only"},
						{"attribute" : "outer"}
					]
				}
			]
		})"),
		request->toString()
	);
}

void GWMessageTest::testParseLastValue()
{
	GWMessage::Ptr message = GWMessage::fromJSON(
	R"({
			"message_type": "last_value_response",
			"id": "495b7a34-d2e7-4cc7-afcc-0690fa5f072a",
			"status": 1,
			"value": 78.5,
			"valid": true
	})");

	CPPUNIT_ASSERT_EQUAL(GWMessageType::LAST_VALUE_RESPONSE, message->type().raw());
	CPPUNIT_ASSERT(!message.cast<GWLastValueResponse>().isNull());

	GWLastValueResponse::Ptr resp = message.cast<GWLastValueResponse>();
	CPPUNIT_ASSERT_EQUAL("495b7a34-d2e7-4cc7-afcc-0690fa5f072a", resp->id().toString());
	CPPUNIT_ASSERT_EQUAL(GWResponse::Status::SUCCESS, resp->status());
	CPPUNIT_ASSERT_EQUAL(78.5, resp->value());
	CPPUNIT_ASSERT(resp->valid());

	message = GWMessage::fromJSON(
	R"({
			"message_type": "last_value_request",
			"id": "495b7a34-d2e7-4cc7-afcc-0690fa5f072a",
			"device_id": "0xfe01020304050607",
			"module_id": "0"
	})");

	CPPUNIT_ASSERT_EQUAL(GWMessageType::LAST_VALUE_REQUEST, message->type().raw());
	CPPUNIT_ASSERT(!message.cast<GWLastValueRequest>().isNull());

	GWLastValueRequest::Ptr request = message.cast<GWLastValueRequest>();
	CPPUNIT_ASSERT_EQUAL("495b7a34-d2e7-4cc7-afcc-0690fa5f072a", request->id().toString());
	CPPUNIT_ASSERT_EQUAL("0xfe01020304050607", request->deviceID().toString());
	CPPUNIT_ASSERT_EQUAL("0", request->moduleID().toString());
}

void GWMessageTest::testCreateLastValue()
{
	GWLastValueResponse::Ptr response(new GWLastValueResponse);
	response->setID(GlobalID::parse("a08c356b-316d-4690-84d4-b77d95b403fe"));
	response->setStatus(GWResponse::Status::SUCCESS);
	response->setValue(80);
	response->setValid(true);

	CPPUNIT_ASSERT_EQUAL(
		jsonReformat(R"({
			"message_type": "last_value_response",
			"id": "a08c356b-316d-4690-84d4-b77d95b403fe",
			"status": 1,
			"value": 80,
			"valid": true
		})"),
		response->toString()
	);

	GWLastValueRequest::Ptr request(new GWLastValueRequest);
	request->setID(GlobalID::parse("a08c356b-316d-4690-84d4-b77d95b403fe"));
	request->setDeviceID(DeviceID::parse("0xfe01020304050607"));
	request->setModuleID(0);

	CPPUNIT_ASSERT_EQUAL(
		jsonReformat(R"({
			"message_type": "last_value_request",
			"id": "a08c356b-316d-4690-84d4-b77d95b403fe",
			"device_id": "0xfe01020304050607",
			"module_id": "0"
		})"),
		request->toString()
	);
}

void GWMessageTest::testParseDeviceList()
{
	GWMessage::Ptr message = GWMessage::fromJSON(
	R"({
			"message_type": "device_list_response",
			"id": "495b7a34-d2e7-4cc7-afcc-0690fa5f072a",
			"status": 1,
			"devices": [
				{"device_id": "0xa15410132465788"},
				{"device_id": "0xa15410132465789"}
			]
	})");

	CPPUNIT_ASSERT_EQUAL(GWMessageType::DEVICE_LIST_RESPONSE, message->type().raw());
	CPPUNIT_ASSERT(!message.cast<GWDeviceListResponse>().isNull());

	GWDeviceListResponse::Ptr response = message.cast<GWDeviceListResponse>();
	CPPUNIT_ASSERT_EQUAL("495b7a34-d2e7-4cc7-afcc-0690fa5f072a", response->id().toString());
	CPPUNIT_ASSERT_EQUAL(GWResponse::Status::SUCCESS, response->status());

	vector<DeviceID> devices = response->devices();
	CPPUNIT_ASSERT_EQUAL("0xa15410132465788", devices[0].toString());
	CPPUNIT_ASSERT_EQUAL("0xa15410132465789", devices[1].toString());
}

void GWMessageTest::testCreateDeviceList()
{
	GWDeviceListResponse::Ptr response(new GWDeviceListResponse);
	response->setID(GlobalID::parse("a08c356b-316d-4690-84d4-b77d95b403fe"));
	response->setStatus(GWResponse::Status::SUCCESS);

	vector<DeviceID> devices;
	devices.push_back(DeviceID::parse("0xa1001234567890a0"));
	devices.push_back(DeviceID::parse("0xa1001234567890a1"));
	response->setDevices(devices);

	CPPUNIT_ASSERT_EQUAL(
		jsonReformat(R"({
			"message_type": "device_list_response",
			"id": "a08c356b-316d-4690-84d4-b77d95b403fe",
			"status": 1,
			"devices": [
				{"device_id": "0xa1001234567890a0"},
				{"device_id": "0xa1001234567890a1"}
			]
		})"),
		response->toString()
	);
}

}

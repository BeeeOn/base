#include <Poco/Net/HTTPSClientSession.h>

#include "net/HTTPUtil.h"

using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;
using namespace std;

HTTPEntireResponse HTTPUtil::makeRequest(
	Poco::Net::HTTPRequest& request,
	const Poco::URI& uri,
	const std::string& msg,
	const Poco::Timespan& timeout)
{
	return makeRequest(request, uri, msg, nullptr, timeout);
}

HTTPEntireResponse HTTPUtil::makeRequest(
	Poco::Net::HTTPRequest& request,
	const Poco::URI& uri,
	const std::string& msg,
	Poco::SharedPtr<SSLClient> sslConfig,
	const Poco::Timespan& timeout)
{
	if (request.getURI() == "/")
		request.setURI(uri.getPathAndQuery());

	return makeRequest(request, uri.getHost(),
		uri.getPort(), msg, sslConfig, timeout);
}

HTTPEntireResponse HTTPUtil::makeRequest(
	Poco::Net::HTTPRequest& request,
	const std::string& host,
	const uint16_t port,
	const std::string& msg,
	const Poco::Timespan& timeout)
{
	return makeRequest(request, host,
		port, msg, nullptr, timeout);
}

HTTPEntireResponse HTTPUtil::makeRequest(
	Poco::Net::HTTPRequest& request,
	const std::string& host,
	const uint16_t port,
	const std::string& msg,
	Poco::SharedPtr<SSLClient> sslConfig,
	const Poco::Timespan& timeout)
{
	SharedPtr<HTTPClientSession> session;

	if (sslConfig.isNull())
		session = new HTTPClientSession(host, port);
	else
		session = new HTTPSClientSession(
			host, port, sslConfig->context());

	if (timeout >= 0)
		session->setTimeout(timeout);

	return sendRequest(session, request, msg);
}

HTTPEntireResponse HTTPUtil::sendRequest(
	SharedPtr<HTTPClientSession> session, HTTPRequest& request, const string& msg)
{
	if (!msg.empty())
		session->sendRequest(request) << msg;
	else
		session->sendRequest(request);

	HTTPEntireResponse response;
	istream& input = session->receiveResponse(response);
	response.readBody(input);

	return response;
}

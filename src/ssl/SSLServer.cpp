#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/NumberParser.h>
#include <Poco/Net/Utility.h>
#include <Poco/Net/SSLManager.h>

#include "di/Injectable.h"
#include "ssl/SSLServer.h"
#include "ssl/RejectCertificateHandler.h"
#include "util/CryptoParams.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SSLServer::SSLServer():
	m_certHandler(new BetterRejectCertificateHandler(true)),
	m_preferServerCiphers(false)
{
}

SSLServer::~SSLServer()
{
}

void SSLServer::setSessionID(const string &id)
{
	m_sessionID = id;
}

void SSLServer::setPreferServerCiphers(bool prefer)
{
	m_preferServerCiphers = prefer;
}

Context::Ptr SSLServer::createContext()
{
	Context::Ptr context = new Context(
		Context::SERVER_USE,
		m_privateKey,
		m_certificate,
		m_caLocation,
		m_verificationMode,
		m_verificationDepth,
		m_loadDefaultCA,
		m_cipherList
	);

	if (trim(m_sessionID).empty()) {
		logger().notice("SSL session ID not set, generating a random one",
				__FILE__, __LINE__);

		m_sessionID = CryptoParams::randomString(16);
	}

	context->enableSessionCache(m_sessionCache, m_sessionID);

	if (m_preferServerCiphers) {
		logger().notice("prefering server ciphers over the client ones",
				__FILE__, __LINE__);

		context->preferServerCiphers();
	}

	return context;
}

BEEEON_OBJECT_BEGIN(BeeeOn, SSLServer)
BEEEON_OBJECT_TEXT("caLocation", &SSLFacility::setCALocation)
BEEEON_OBJECT_TEXT("loadDefaultCA", &SSLFacility::setLoadDefaultCA)
BEEEON_OBJECT_TEXT("privateKey", &SSLFacility::setPrivateKey)
BEEEON_OBJECT_TEXT("passphrase", &SSLFacility::setPassphrase)
BEEEON_OBJECT_TEXT("certificate", &SSLFacility::setCertificate)
BEEEON_OBJECT_TEXT("verificationMode", &SSLFacility::setVerificationMode)
BEEEON_OBJECT_NUMBER("verificationDepth", &SSLFacility::setVerificationDepth)
BEEEON_OBJECT_TEXT("cipherList", &SSLFacility::setCipherList)
BEEEON_OBJECT_TEXT("sessionCache", &SSLFacility::setSessionCache)
BEEEON_OBJECT_TEXT("sessionID", &SSLServer::setSessionID)
BEEEON_OBJECT_TEXT("disabledProtocols", &SSLFacility::setDisabledProtocols)
BEEEON_OBJECT_NUMBER("preferServerCiphers", &SSLServer::setPreferServerCiphers)
BEEEON_OBJECT_NUMBER("extendedCertificateVerification", &SSLFacility::setExtendedCertificateVerification)
BEEEON_OBJECT_HOOK("done", &SSLFacility::initContext)
BEEEON_OBJECT_END(BeeeOn, SSLServer)

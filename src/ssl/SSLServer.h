#ifndef BEEEON_SSL_SERVER_H
#define BEEEON_SSL_SERVER_H

#include <Poco/SharedPtr.h>
#include <Poco/Net/InvalidCertificateHandler.h>

#include "ssl/SSLFacility.h"

namespace BeeeOn {

class SSLServer : public SSLFacility {
public:
	SSLServer();
	~SSLServer();

	void setSessionID(const std::string &id);

protected:
	Poco::Net::Context::Ptr createContext() override;

protected:
	Poco::SharedPtr<Poco::Net::InvalidCertificateHandler>
		m_certHandler;
	std::string m_sessionID;
};

}

#endif

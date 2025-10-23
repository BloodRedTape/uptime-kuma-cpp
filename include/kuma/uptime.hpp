#pragma once

#include "provider/provider.hpp"

namespace Kuma {

class Uptime {
	std::unique_ptr<Provider> m_Provider;
	std::string m_Token;
public:
	Uptime(std::unique_ptr<Provider> &&provider, const std::string &token);

	Uptime(std::unique_ptr<HttpClient> &&client, const std::string &url, const std::string &token);

	void Push();
};

}//namespace Kuma::
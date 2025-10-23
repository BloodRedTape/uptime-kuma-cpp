#pragma once


#include "kuma/net/client.hpp"

namespace Kuma{

	class BoostHttpClient : public HttpClient {
	public:
		void MakeRequest(HttpClientMethod method, const std::string &hostname, const std::string &path, const std::map<std::string, std::string> &headers, std::string&& body, CompleteCallbackType &&complete)override;
	};

}//namespace Kuma::
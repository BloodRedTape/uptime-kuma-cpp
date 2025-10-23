#include "kuma/uptime.hpp"
#include "kuma/provider/worker_provider.hpp"

Kuma::Uptime::Uptime(std::unique_ptr<Kuma::Provider> &&provider, const std::string &token):
	m_Provider(std::move(provider)),
	m_Token(token)
{}

Kuma::Uptime::Uptime(std::unique_ptr<Kuma::HttpClient> &&client, const std::string &url, const std::string &token):
	m_Provider(std::make_unique<WorkerProvider>(std::move(client), url)),
	m_Token(token)
{}


void Kuma::Uptime::Push() {
	m_Provider->Push(m_Token);
}

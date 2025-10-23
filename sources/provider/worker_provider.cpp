#include "kuma/provider/worker_provider.hpp"

Kuma::WorkerProvider::WorkerProvider(std::unique_ptr<HttpClient> &&client,
                                               const std::string &api_url)
    : m_Client(std::move(client)),
      m_ApiUrl(api_url),
      m_FlushInterval(std::chrono::seconds(5))
{
    m_WorkerThread = std::thread(&Kuma::WorkerProvider::WorkerLoop, this);
}

Kuma::WorkerProvider::~WorkerProvider()
{
    m_StopFlag = true;
    m_Condition.notify_all();

    if (m_WorkerThread.joinable())
        m_WorkerThread.join();
}

void Kuma::WorkerProvider::Push(const std::string &token)
{
    {
        std::lock_guard<std::mutex> lock(m_PushMutex);
        m_PushTokens.emplace_back(token);
    }
    m_Condition.notify_one();  // wake up flush thread
}

void Kuma::WorkerProvider::WorkerLoop()
{
    std::unique_lock<std::mutex> lock(m_ConditionMutex);
    while (!m_StopFlag.load()) {
        m_Condition.wait_for(lock, m_FlushInterval, [this]() {
            return m_StopFlag || AnyPending();
        });

        if (m_StopFlag)
            break;

        if (AnyPending()) {
            CopyAndFlushBatch();
        }
    }
}

void Kuma::WorkerProvider::CopyAndFlushBatch()
{
    std::vector<std::string> tokens;
    {
        std::lock_guard<std::mutex> lock(m_PushMutex);
        if (m_PushTokens.empty())
            return;
        tokens.swap(m_PushTokens);
    }

    m_PendingSends.fetch_add(1);
    
    for(const auto &token: tokens){
        m_Client->MakeRequest(
            Kuma::HttpClientMethod::Post,
            m_ApiUrl,
            "/api/push/" + token,
            {},
            {},
            [this](std::int32_t status) {
            if (status >= 200 && status < 300) {
                m_LogFunction(Verbosity::Verbose, "Push done.");
            } else {
                m_LogFunction(Verbosity::Error, "HTTP " + std::to_string(status) + " while push.");
            }
            m_PendingSends.fetch_sub(1);
        });
    }
}
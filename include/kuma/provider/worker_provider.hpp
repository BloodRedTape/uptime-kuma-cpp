#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "provider.hpp"
#include "kuma/net/client.hpp"

namespace Kuma{

    class WorkerProvider : public Provider {
    private:
        std::unique_ptr<HttpClient> m_Client;
        std::string m_ApiUrl;
    
        std::vector<std::string> m_PushTokens;
        mutable std::mutex m_PushMutex;

        std::atomic<int> m_PendingSends{0};
        std::atomic<bool> m_StopFlag{false};

        std::thread m_WorkerThread;
        std::condition_variable m_Condition;
        std::mutex m_ConditionMutex;

        std::chrono::seconds m_FlushInterval;

        LogFunctionType m_LogFunction = DefaultLogFunction;
    public:
        WorkerProvider(std::unique_ptr<HttpClient> &&client, const std::string &api_url);

        ~WorkerProvider();

        void Push(const std::string &token) override;

        bool AnyPending()const{ return m_PushTokens.size(); }

        void SetLog(LogFunctionType&& log) { m_LogFunction = std::move(log); }

    private:
        void WorkerLoop();
        void CopyAndFlushBatch();

    };

}//namespace Aptabase::
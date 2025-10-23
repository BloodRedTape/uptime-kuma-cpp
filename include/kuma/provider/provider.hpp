#pragma once

#include "kuma/net/client.hpp"

namespace Kuma{

	enum class Verbosity {
		Verbose,
		Display,
		Info,
		Warning,
		Error
	};

	extern const char *ToString(Verbosity verbosity);

	class Provider {
	public:	
		using LogFunctionType = std::function<void(Verbosity, const std::string &)>;
	public:
		static void DefaultLogFunction(Verbosity verbosity, const std::string &message);

		virtual ~Provider() = default;

		virtual void Push(const std::string &token) = 0;

		virtual void SetLog(LogFunctionType &&log) = 0;
	};

}//namespace Kuma::
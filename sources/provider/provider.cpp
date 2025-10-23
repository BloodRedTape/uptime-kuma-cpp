#include "kuma/provider/provider.hpp"
#include <iostream>

const char *Kuma::ToString(Verbosity verbosity) {
	switch (verbosity) {
	case Verbosity::Verbose: return "Verbose";
	case Verbosity::Display: return "Display";
	case Verbosity::Info: return "Info";
	case Verbosity::Warning: return "Warning";
	case Verbosity::Error: return "Error";
	}

	return "Unknown";
}

void Kuma::Provider::DefaultLogFunction(Verbosity verbosity, const std::string& message) {
	std::cout << ToString(verbosity) << ":" << message << '\n';
}

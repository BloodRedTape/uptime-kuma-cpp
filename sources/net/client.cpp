#include "kuma/net/client.hpp"
#include <cassert>

const char* Kuma::ToString(HttpClientMethod method){
	if(method == HttpClientMethod::Post)
		return "POST";

	assert(false);
	return "<None>";
}

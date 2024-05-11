#include "tolua_network.h"
#define BIND_TO_SOL
#include "rust_env.h"

void tolua_network(sol::state& lua)
{
	auto network = lua["network"].get_or_create<sol::table>();
	network.new_usertype<RustEnv>("RustEnv",
		sol::constructors<RustEnv(uint32_t)>(),
		"newHttpClient", &RustEnv::newHttpClient,
		"httpGet", &RustEnv::httpGet,
		"httpPost", &RustEnv::httpPost,
		"addHeader", &RustEnv::addHeader,
		"clearHeader", &RustEnv::clearHeader,
		"addParam", &RustEnv::addParam,
		"clearParam", &RustEnv::clearParam,
		"setClearExpiresEnabled", &RustEnv::setClearExpiresEnabled,
		"getRequestState", &RustEnv::getRequestState,
		"getRequestError", &RustEnv::getRequestError,
		"removeRequest", &RustEnv::removeRequest,
		"getResponseCookies", &RustEnv::getResponseCookies,
		"getResponseHeaders", &RustEnv::getResponseHeaders,
		"getRequestResponse", &RustEnv::getRequestResponse_tolua,
		"wsConnect", &RustEnv::wsConnect,
		"wsSend", &RustEnv::wsSend,
		"wsRemove", &RustEnv::wsRemove,
		"wsMessage", &RustEnv::wsMessage_tolua
		);
}

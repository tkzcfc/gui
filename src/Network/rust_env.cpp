#define BIND_TO_SOL

#include "rust_env.h"
#include <assert.h>

RustEnv::RustEnv(uint32_t thread_count)
	: m_tokio(NULL)
	, m_seed(1)
{
	m_tokio = rust_net_tokio_new(thread_count);
}

RustEnv::~RustEnv()
{
	for (auto& it : m_clientMap)
	{
		rust_net_http_client_free(it.second);
	}
	m_clientMap.clear();

	for (auto& it : m_wsMap)
	{
		rust_net_ws_free(it.second);
	}
	m_wsMap.clear();

	if (m_tokio)
	{
		rust_net_tokio_free(m_tokio);
		m_tokio = NULL;
	}
}

uint32_t RustEnv::newHttpClient(bool brotli, bool cookie_store)
{
	auto client = rust_net_http_client_new(brotli, cookie_store);
	if (client == NULL)
		return 0;

	m_seed++;

	m_clientMap[m_seed] = client;

	return m_seed;
}

uint64_t RustEnv::httpGet(uint32_t client, const char* url)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		return rust_net_http_get(m_tokio, it->second, url);
	}
	assert(false);
	return 0;
}

uint64_t RustEnv::httpPost(uint32_t client, const char* url, const char* data, size_t length)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		return rust_net_http_post(m_tokio, it->second, url, (const uint8_t*)data, length);
	}
	assert(false);
	return 0;
}

void RustEnv::addHeader(uint32_t client, const char* key, const char* value)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		rust_net_http_add_header(it->second, key, value);
		return;
	}
	assert(false);
}

void RustEnv::clearHeader(uint32_t client)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		rust_net_http_clear_header(it->second);
	}
	assert(false);
}

void RustEnv::addParam(uint32_t client, const char* key, const char* value)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		rust_net_http_add_param(it->second, key, value);
		return;
	}
	assert(false);
}

void RustEnv::clearParam(uint32_t client)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		rust_net_http_clear_param(it->second);
	}
	assert(false);
}

void RustEnv::setClearExpiresEnabled(uint32_t client, bool value)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		rust_net_http_set_clear_expires_enabled(it->second, value);
	}
	assert(false);
}

int32_t RustEnv::getRequestState(uint32_t client, uint64_t key)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		return rust_net_http_get_request_state(it->second, key);
	}
	assert(false);
	return -2;
}

std::string RustEnv::getRequestError(uint32_t client, uint64_t key)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		auto err = rust_net_http_get_request_error(it->second, key);
		if (err == NULL)
			return "";

		std::string ret(err);

		rust_net_http_free_string(err);

		return ret;
	}
	assert(false);
	return "";
}

std::string RustEnv::getResponseCookies(uint32_t client, uint64_t key)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		auto err = rust_net_http_get_response_cookies(it->second, key);
		if (err == NULL)
			return "{}";

		std::string ret(err);

		rust_net_http_free_string(err);

		return ret;
	}
	assert(false);
	return "{}";
}

std::string RustEnv::getResponseHeaders(uint32_t client, uint64_t key)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		auto err = rust_net_http_get_response_headers(it->second, key);
		if (err == NULL)
			return "{}";

		std::string ret(err);

		rust_net_http_free_string(err);

		return ret;
	}
	assert(false);
	return "{}";
}

void RustEnv::removeRequest(uint32_t client, uint64_t key)
{
	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		rust_net_http_remove_request(it->second, key);
	}
}

RequestResponseLua RustEnv::getRequestResponse(uint32_t client, uint64_t key)
{
	RequestResponseLua ret;

	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		auto response = rust_net_http_get_request_response(it->second, key);

		if (response.data && response.len > 0)
		{
			ret.data.append((const char*)response.data, response.len);
		}
		ret.status = response.status;
		ret.version = response.version;

		rust_net_http_free_request_response(response);
	}

	return ret;
}

uint32_t RustEnv::wsConnect(const char* url, const char* cookies)
{
	auto ctx = rust_net_ws_connect(m_tokio, url, cookies);
	if (ctx) 
	{
		m_seed++;
		m_wsMap[m_seed] = ctx;
		return m_seed;
	}
	return 0;
}

void RustEnv::wsSend(uint32_t key, const char* data, size_t length)
{
	auto it = m_wsMap.find(key);
	if (it != m_wsMap.end())
	{
		rust_net_ws_send(it->second, (const uint8_t*)data, length);
	}
}

void RustEnv::wsRemove(uint32_t key)
{
	auto it = m_wsMap.find(key);
	if (it != m_wsMap.end())
	{
		rust_net_ws_free(it->second);
		m_wsMap.erase(it);
	}
}

#ifdef BIND_TO_SOL
sol::table RustEnv::getRequestResponse_tolua(sol::this_state ts, uint32_t client, uint64_t key)
{
	sol::state_view lua(ts);
	sol::table data = lua.create_table();

	auto it = m_clientMap.find(client);
	if (it != m_clientMap.end())
	{
		auto response = rust_net_http_get_request_response(it->second, key);

		data["status"] = response.status;
		data["version"] = response.version;

		if (response.data && response.len > 0)
			data["data"] = std::string((const char*)response.data, response.len);
		else
			data["data"] = "";

		rust_net_http_free_request_response(response);
	}
	else
	{
		assert(false);
		data["status"] = 0;
		data["version"] = 0;
		data["data"] = "";
	}

	return data;
}

sol::table RustEnv::wsMessage_tolua(sol::this_state ts, uint32_t key)
{
	sol::state_view lua(ts);
	sol::table data = lua.create_table();

	auto it = m_wsMap.find(key);
	if (it != m_wsMap.end())
	{
		auto message = rust_net_ws_get_message(it->second);

		data["message_type"] = message.message_type;
		if (message.data && message.len > 0)
			data["data"] = std::string((const char*)message.data, message.len);
		else
			data["data"] = "";

		rust_net_ws_free_message(message);
	}
	else
	{
		assert(false);
		data["message_type"] = -1;
		data["data"] = "";
	}

	return data;
}

#endif
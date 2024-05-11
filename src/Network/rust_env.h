#pragma once

#include "rust_net.h"
#include <string>

#ifdef BIND_TO_SOL
#include "sol/sol.hpp"
#endif

struct RequestResponseLua
{
	std::string data;
	uint32_t status;
	int32_t version;
};

class RustEnv final
{
public:

	RustEnv(uint32_t thread_count);

	~RustEnv();

	////////////////////////////////////////////////////////////////// http //////////////////////////////////////////////////////////////////

	uint32_t newHttpClient(bool brotli, bool cookie_store);

	uint64_t httpGet(uint32_t client, const char* url);

	uint64_t httpPost(uint32_t client, const char* url, const char* data, size_t length);

	void addHeader(uint32_t client, const char* key, const char* value);

	void clearHeader(uint32_t client);

	void addParam(uint32_t client, const char* key, const char* value);

	void clearParam(uint32_t client);

	void setClearExpiresEnabled(uint32_t client, bool value);

	int32_t getRequestState(uint32_t client, uint64_t key);

	std::string getRequestError(uint32_t client, uint64_t key);

	std::string getResponseCookies(uint32_t client, uint64_t key);

	std::string getResponseHeaders(uint32_t client, uint64_t key);

	void removeRequest(uint32_t client, uint64_t key);

	RequestResponseLua getRequestResponse(uint32_t client, uint64_t key);

	////////////////////////////////////////////////////////////////// websocket //////////////////////////////////////////////////////////////////

	uint32_t wsConnect(const char* url, const char* cookies);

	void wsSend(uint32_t key, const char* data, size_t length);

	void wsRemove(uint32_t key);

#ifdef BIND_TO_SOL
	sol::table getRequestResponse_tolua(sol::this_state ts, uint32_t client, uint64_t key);

	/*
	* message_type:
		enum WsMessage {
		    // 没有消息
		    None,
		    // 连接成功
		    ConnectSuccess,
		    // 连接失败
		    ConnectFailed,
		    // 断开连接
		    Disconnect,
		    // 收到Ping
		    RecvPing,
		    // 收到Pong
		    RecvPong,
		    // 收到文本消息
		    RecvText,
		    // 收到二进制消息
		    RecvBinary,
		}
	*/
	sol::table wsMessage_tolua(sol::this_state ts, uint32_t key);
#endif
		 
private:
	TokioContext* m_tokio;
	std::unordered_map<uint32_t, ClientContext*> m_clientMap;
	std::unordered_map<uint32_t, WsContext*> m_wsMap;
	uint32_t m_seed;
};

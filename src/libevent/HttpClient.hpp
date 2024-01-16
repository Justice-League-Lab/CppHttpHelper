#pragma once
#include <thread>
#include <map>
#include <atomic>
#include <event2/event.h>
#include <memory>
#include <bbt/buffer/Buffer.hpp>
#include "detail/Define.hpp"

namespace bbt::http::ev
{

class HttpClient;

typedef std::function<void(CURL* req, buffer::Buffer& body)> RespHandler;

struct RequestData
{
    RespHandler m_response_callback;
    CURL*       m_curl;
    curl_slist* m_list;
    RequestId   m_id;
};

// 请求携带的私有数据
struct RequestPrivData
{
    HttpClient* m_pthis{NULL};
    uint64_t    m_request_id{0};
};

class HttpClient
{
    friend size_t CurlWrite(void* buf, size_t size, size_t nmemb, void* arg);
public:
    HttpClient(event_base* io_ctx);
    ~HttpClient();

    ErrOpt PostReq(const char* url, buffer::Buffer& body, RespHandler cb);

    void RunOnce();
    void TimeTick();
protected:
    /* 50ms 触发一次（有误差） */
    void __OnTime50Ms();
    void __RegistTimerEvent();
    void __UnRegistTimerEvent();
private:
    event_base*                 m_io_ctx{NULL};
    event*                      m_timer{NULL};
    CURLM*                      m_multi_conn{NULL};
    volatile bool               m_running{true};

    std::atomic_int             m_running_handles{0};
    std::map<RequestId, std::shared_ptr<RequestData>> 
                                m_request_wait_map; 
};



} // namespace bbt::http::ev

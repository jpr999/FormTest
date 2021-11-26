#include <atomic>
#include <fcgiapp.h>
#include <fmt/printf.h>
#include <map>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <thread>

#include "include/stringgenlib.h"
#include "include/utils.h"

const char* const sockpath = ":5862";

void* start_fcgi_worker(void* arg);
bool gen_data(size_t i);

std::mutex g_mutex;
struct FCGI_Info
{
    int fcgi_fd;
    int count;
};

static std::string strHtml = "<!DOCTYPE html>"
                             "<html lang=\"en\">"
                             "  <head>"
                             "    <meta charset=\"utf-8\">"
                             "    <title>Title of the document</title>"
                             "   </head>"
                             "  <body>"
                             "    <p style=\"color:red;\">好世界</p>"
                             "    <p style=\"color:purple;\">Help %s</p>"
                             "  </body>"
                             "</html>";

int main(void)
{
    std::locale::global(std::locale(""));
    int fcgifd = FCGX_OpenSocket(sockpath, 128);

    chmod(sockpath, 0777);

    if(0 > fcgifd)
    {
        printf("Error opening socket\n");
        exit(1);
    }

    const unsigned int n_threads = 20;

    std::thread threads[n_threads];
    FCGI_Info info[n_threads];

    for(unsigned int i = 0; i < n_threads; i++)
    {
        info[i].fcgi_fd = fcgifd;
        info[i].count = i;
        threads[i] = std::thread(start_fcgi_worker, &info[i]);
    }

    // Wait indefinitely
    for(unsigned int i = 0; i < n_threads; i++)
    {
        threads[i].join();
    }

    return 0;
}

#define get_param(r, KEY) FCGX_GetParam(KEY, r.envp)

void* start_fcgi_worker(void* arg)
{

    struct FCGI_Info* info = (struct FCGI_Info*)arg;
    FCGX_Init();
    FCGX_Request request;

    FCGX_InitRequest(&request, info->fcgi_fd, 0);
    while(1)
    {
        FCGX_Accept_r(&request);
        std::string strVal;
        char* pM(nullptr);
        char* pQ(nullptr);

        pM = FCGX_GetParam("REQUEST_METHOD", request.envp);
        pQ = FCGX_GetParam("QUERY_STRING", request.envp);

        if(nullptr != pQ)
        {
            std::string strQ(pQ);
            strVal = fmt::sprintf(strHtml, strQ);
        }

        gen_data(2000);

        std::string str = "Content-type: text/html;  charset=utf-8;\r\n\r\n";
        FCGX_PutStr(str.c_str(), str.length(), request.out);
        FCGX_PutStr(strVal.c_str(), utf8_strlen(strVal), request.out);
        FCGX_Finish_r(&request);
    }
}

bool gen_data(size_t i)
{
    std::lock_guard<std::mutex> guard(g_mutex);

    static int ic(0);
    t_exec a(fmt::sprintf("\n\ncount:%d  time to process map:{}\n", ic));
    ic++;

    std::map<std::string, std::string> m;
    std::vector<std::string> v;
    while(m.size() != i)
    {
        std::string key(generate(50));
        m.insert(std::pair(key, generate(3000)));
        v.emplace_back(key);
    }

    for(const auto& s : v)
    {
        const auto p = m.find(s);
        if(p != m.end())
        {
            if(p->first.length() != 50)
                fmt::printf(fmt::format(FMT_STRING("key:{:s}\n"), p->first));
        }
        else
        {}
    }

    auto m1 = m;
    return true;
}

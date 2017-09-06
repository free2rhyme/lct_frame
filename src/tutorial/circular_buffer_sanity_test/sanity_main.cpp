/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        sanity_main.cpp
 * @version     1.0
 * @date        Aug 18, 2017 11:09:16 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_log.h"
#include <string>
#include <thread>
#include <iostream>
#include <future>
#include <chrono>
#include <unistd.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include "lct_circular_buffer.h"

typedef std::chrono::system_clock LctClockType;
typedef std::chrono::time_point<LctClockType> LctTimePointType;
typedef std::chrono::milliseconds LctTimeValueType;

const std::string LctCurrentTime()  //HH:MM:SS
{
    std::time_t now = LctClockType::to_time_t(LctClockType::now());
    struct tm tstruct;
    tstruct = *localtime_r(&now, &tstruct);
    char buf[80] = { 0 };
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

#define log_trace() std::cout << LctCurrentTime() << "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << "\t" << __LINE__ << "\t" << getpid() << "\t" << std::this_thread::get_id() << std::endl
#define log_debug(x) std::cout << LctCurrentTime() << "\t" << __LINE__ << "\t" << __PRETTY_FUNCTION__ << "\t" << __LINE__ << "\t" << x << std::endl
#define log_object() printf("%d: Tracer(%p), %s, m_id(%d)\n", __LINE__, this, __PRETTY_FUNCTION__, m_id)

class CTracer
{
public:
    CTracer() noexcept(true)
    {
        log_object();
        m_ptr = new char[BUFFER_SIZE];
    }

    explicit CTracer(const int32_t i) noexcept(true): m_id(i)
    {
        m_ptr = new char[BUFFER_SIZE];
        log_object();
    }

    ~CTracer() noexcept(true)
    {
        log_object();
        if (m_ptr != nullptr) {
            delete[] m_ptr;
        }
    }

    CTracer(const CTracer& that) noexcept(true): m_id(that.m_id)
    {
        m_ptr = new char[BUFFER_SIZE];
        printf("%d: Tracer(%p), that(%p), m_ptr(%p), that.m_ptr(%p), %s, m_id(%d)\n", __LINE__, this, &that, m_ptr,
                that.m_ptr, __PRETTY_FUNCTION__, m_id);
    }

    CTracer& operator=(const CTracer& that) noexcept(true)
    {
        m_id = that.m_id;
        printf("%d: Tracer(%p), that(%p), m_ptr(%p), that.m_ptr(%p), %s, m_id(%d)\n", __LINE__, this, &that, m_ptr,
                that.m_ptr, __PRETTY_FUNCTION__, m_id);
        return *this;
    }

    CTracer(CTracer&& that) noexcept(true) :
            m_id(that.m_id)
    {
        m_ptr = new char[BUFFER_SIZE];
        that.m_ptr = nullptr;
        that.m_id = -1;
        printf("%d: Tracer(%p), that(%p), m_ptr(%p), that.m_ptr(%p), %s, m_id(%d)\n", __LINE__, this, &that, m_ptr,
                that.m_ptr, __PRETTY_FUNCTION__, m_id);
    }

    CTracer& operator=(CTracer&& that) noexcept(true)
    {
        m_id = that.m_id;
        that.m_id = -1;
        printf("%d: Tracer(%p), that(%p), m_ptr(%p), that.m_ptr(%p), %s, m_id(%d)\n", __LINE__, this, &that, m_ptr,
                that.m_ptr, __PRETTY_FUNCTION__, m_id);
        return *this;
    }

    void display() const
    {
        printf("Tracer(%p), m_id(%d)\n", this, m_id);
    }

    int32_t size() const
    {
        return BUFFER_SIZE + sizeof(m_id);
    }

public:
    static constexpr int32_t BUFFER_SIZE = 128 * 1024;
    int m_id = 0;
    char* m_ptr = nullptr;
};

typedef std::vector<CTracer> CTracerVec;

int main()
{

    CLctCircularBuffer<CTracer> buffer(20);

    buffer.emplace(100);
    buffer.emplace(200);
    buffer.emplace(300);
    buffer.emplace(400);
    buffer.emplace(500);

    std::cout << "\n\n" << "Going to iterate buffer:::::" << std::endl;
    LCT_ERR_CODE errCode = buffer.iterate([](const CTracer& obj) {
        obj.display();
    });
    std::cout << "Finished iterator" << std::endl;

    CTracer ttracer;
    buffer.pop(ttracer);
    ttracer.display();

    errCode = buffer.iterate([](const CTracer& obj) {
        obj.display();
    });

    return 0;
}

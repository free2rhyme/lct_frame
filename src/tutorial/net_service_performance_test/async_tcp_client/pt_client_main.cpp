/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        pt_client_main.cpp
 * @version     1.0
 * @date        Aug 18, 2017 11:09:08 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_log.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "lct_net_connect.h"

/* Returns microseconds since epoch */
uint64_t timestamp_now()
{
    return std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::microseconds(1);
}

void lct_net_benchmark() {
    int64_t clientCount = 100;

    LOG_DEBUG_MARK();

    while(clientCount-- > 0) {
        CLctNetConnect client("127.0.0.1", 9090);
        constexpr int64_t iterations = 1000;
        uint64_t begin = timestamp_now();
        for (int64_t i = 1; i <= iterations; ++i ) {
            const std::string val = std::to_string(1) + 'V';

            client.asyncWrite(val, []() {
            });
        }
        uint64_t end = timestamp_now();
        long int latency = (end - begin) / (iterations * 3);
        std::cout << "Latency = " << latency << " nanoseconds";
    }

    LOG_DEBUG_MARK();
}

template < typename Function >
void run_benchmark(Function && f, int thread_count)
{
    LOG_DEBUG << "Thread count: " << thread_count;

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i)
    {
        threads.emplace_back(f);
    }
    for (int i = 0; i < thread_count; ++i)
    {
        threads[i].join();
    }
}

int main()
{
    LCT_LOG->init("/tmp/log");

    constexpr int32_t workerCount = 40;

    for (auto threads : { workerCount })
        run_benchmark(lct_net_benchmark, threads);

    sleep(10);

    LOG_DEBUG_MARK();

    return 0;
}




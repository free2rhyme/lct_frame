#include "lct_log.h"
#include <chrono>
#include <thread>
#include <vector>
#include <atomic>
#include <cstdio>

/* Returns microseconds since epoch */
uint64_t timestamp_now()
{
   return std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::microseconds(1);
}

void nanolog_benchmark()
{
   int const iterations = 5000;
   char const * const benchmark = "benchmark";
   uint64_t begin = timestamp_now();
   for (int i = 0; i < iterations; ++i){
      LOG_WARNG << "Logging information Booting up lowcost Service Static Verify ......" << benchmark << i << 0 << 'K' << -42.42;
      LOG_INFOR << "Logging information Booting up lowcost Service Static Verify ......" << benchmark << i << 0 << 'K' << -42.42;
      LOG_CRITC << "Logging information Booting up lowcost Service Static Verify ......" << benchmark << i << 0 << 'K' << -42.42;
   }
   uint64_t end = timestamp_now();
   long int avg_latency = (end - begin) / (iterations * 3);
   printf("\tAverage lyLog Latency = %ld nanoseconds\n", avg_latency);
}

template < typename Function >
void run_benchmark(Function && f, int thread_count)
{
   printf("Thread count: %d\n", thread_count);
   std::vector < std::thread > threads;
   for (int i = 0; i < thread_count; ++i) {
      threads.emplace_back(f);
   }
   for (int i = 0; i < thread_count; ++i) {
      threads[i].join();
   }
}

int main()
{
   // Ring buffer size is passed as 10 mega bytes.
   // Since each log line = 256 bytes, thats 40960 slots.

   LCT_LOG->init("/lab/lylog/logs/");

   constexpr int32_t workerCount = 20;

   for (auto threads : { workerCount }) {
      run_benchmark(nanolog_benchmark, threads);
   }

   sleep(10);
   return 0;
}


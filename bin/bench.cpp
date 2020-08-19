/**
 * @file benchmark of symcoro::CoroWorker
 */
#include <boost/context/continuation.hpp>
#include <boost/coroutine2/all.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>

#include "symcoro/symcoro.hpp"

constexpr static int kBenchmarkMills = 5000;  //< benchmark for 5 ms
int main()
{
    symcoro::CoroWorker worker;
    std::atomic<bool> stop{false};
    std::atomic<int> cnt{0};
    // timer to wait until kBenchmarkMills elaps
    std::thread timer([&]() {
        auto start = std::chrono::steady_clock::now();
        while (true)
        {
            auto now = std::chrono::steady_clock::now();
            auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(
                           now - start)
                           .count();
            if (cnt >= kBenchmarkMills)
            {
                stop = true;
                break;
            }
            usleep(500);
        }
    });
    for (int i = 0; i < 20; ++i)
    {
        worker.Register([&](symcoro::cc_t &&sink) {
            // std::cout << "from job i" << std::endl;
            while (!stop)
            {
                sink = sink.resume();
                cnt.fetch_add(1, std::memory_order_relaxed);
            }
            return std::move(sink);
        });
    }

    // block until all the jobs are finished.
    worker.Serve();
    std::cout << "count is " << cnt << " for " << kBenchmarkMills << " ms"
              << std::endl;
    std::cout << "Latency is " << 1.0 * kBenchmarkMills * 1000 * 1000 / cnt
              << " ns" << std::endl;

    timer.join();
    return 0;
}
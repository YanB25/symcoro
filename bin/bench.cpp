#include <boost/context/continuation.hpp>
#include <boost/coroutine2/all.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>

#include "symcoro/symcoro.hpp"
int main()
{
    symcoro::CoroWorker worker;
    std::atomic<bool> stop{false};
    std::atomic<int> cnt{0};
    std::thread timer([&]() {
        auto start = std::chrono::steady_clock::now();
        while (true)
        {
            auto now = std::chrono::steady_clock::now();
            auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(
                           now - start)
                           .count();
            if (cnt >= 1000)
            {
                stop = true;
                break;
            }
            usleep(500);
        }
    });
    for (int i = 0; i < 100; ++i)
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
    std::cout << "count is " << cnt << std::endl;

    timer.join();
    return 0;
}
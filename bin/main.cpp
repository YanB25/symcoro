#include <boost/context/continuation.hpp>
#include <boost/coroutine2/all.hpp>
#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <thread>

#include "symcoro/symcoro.hpp"
using cc_t = boost::context::continuation;
int main()
{
    symcoro::CoroWorker worker;

    // register a job
    worker.Register([&](symcoro::cc_t &&sink) {
        std::cout << "from job 1" << std::endl;
        return std::move(sink);
    });
    // register yet another job
    worker.Register([&](symcoro::cc_t &&sink) {
        std::cout << "from job 2" << std::endl;
        // Switch away and go back here later
        sink = sink.resume();
        std::cout << "from job 2 again" << std::endl;
        return std::move(sink);
    });
    // register yet another job.
    worker.Register([&](symcoro::cc_t &&sink) {
        std::cout << "from job 3" << std::endl;
        return std::move(sink);
    });

    // block until all the jobs are finished.
    worker.Serve();
}
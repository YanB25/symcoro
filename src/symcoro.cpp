#include "symcoro/symcoro.hpp"

#include <algorithm>
#include <boost/context/continuation.hpp>
#include <functional>
namespace symcoro
{
CoroWorker::CoroWorker(CoroWorker &&rhs)
{
    *this = std::move(rhs);
}

CoroWorker &CoroWorker::operator=(CoroWorker &&rhs)
{
    works_.swap(rhs.works_);
    return *this;
}

void CoroWorker::Register(Job job)
{
    works_.emplace(boost::context::callcc([&](cc_t &&sink) {
        sink = sink.resume();
        // two;
        return job(std::move(sink));
    }));
}

void CoroWorker::Serve()
{
    while (!works_.empty())
    {
        auto &ctx = works_.front();
        auto new_ctx = ctx.resume();
        works_.pop();
        if (new_ctx)
        {
            works_.emplace(std::move(new_ctx));
        }
    }
}
}  // namespace symcoro
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
    std::queue<cc_t> continuations;
    for (int i = 0; i < 3; ++i)
    {
        continuations.emplace(boost::context::callcc([&, i](cc_t &&sink) {
            sink = sink.resume();
            std::cout << "hei " << i << std::endl;
            if (!continuations.empty())
            {
                if (continuations.front())
                {
                    auto new_ctx =
                        continuations.front().resume_with([&](cc_t &&sink) {
                            continuations.pop();
                            return std::move(sink);
                        });
                    continuations.emplace(std::move(new_ctx));
                }
                else
                {
                    continuations.pop();
                }
            }
            std::cout << "hei " << i << std::endl;
            if (!continuations.empty())
            {
                if (continuations.front())
                {
                    auto new_ctx =
                        continuations.front().resume_with([&](cc_t &&sink) {
                            continuations.pop();
                            return std::move(sink);
                        });
                    continuations.emplace(std::move(new_ctx));
                }
                else
                {
                    continuations.pop();
                }
            }
            std::cout << "hei " << i << std::endl;
            if (!continuations.empty())
            {
                if (continuations.front())
                {
                    auto new_ctx =
                        continuations.front().resume_with([&](cc_t &&sink) {
                            continuations.pop();
                            return std::move(sink);
                        });
                    continuations.emplace(std::move(new_ctx));
                }
                else
                {
                    continuations.pop();
                }
            }
            return std::move(sink);
        }));
    }
    while (!continuations.empty())
    {
        if (continuations.front())
        {
            std::cout << "Iuuse round" << std::endl;
            continuations.front().resume_with([&](cc_t &&sink) {
                continuations.pop();
                return std::move(sink);
            });
        }
        else
        {
            continuations.pop();
        }
    }
    std::cout << continuations.size() << std::endl;
}
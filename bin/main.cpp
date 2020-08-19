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
    std::cout << symcoro::hello() << std::endl;
}
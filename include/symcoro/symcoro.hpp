#ifndef SYMCORO_H
#define SYMCORO_H
#include <boost/context/continuation.hpp>
#include <queue>
#include <string>
namespace symcoro
{
using cc_t = boost::context::continuation;
class CoroWorker
{
public:
    using Job = std::function<cc_t(cc_t &&)>;
    CoroWorker() = default;
    CoroWorker(CoroWorker &&rhs);
    CoroWorker(const CoroWorker &) = delete;
    CoroWorker &operator=(CoroWorker &&rhs);
    CoroWorker &operator=(const CoroWorker &rhs) = delete;
    ~CoroWorker() = default;

    void Register(Job job);
    void Serve();

private:
    std::queue<cc_t> works_;
};
std::string hello();
}  // namespace symcoro
#endif
/**
 * @file main file to define CoroWorker
 */
#ifndef SYMCORO_H
#define SYMCORO_H
#include <boost/context/continuation.hpp>
#include <queue>
#include <string>
/**
 * symcoro is the main namespace where everything is in
 */
namespace symcoro
{
using cc_t = boost::context::continuation;  //< continuation

/**
 * @brief CoroWorker is the worker based on coroutine to schedule @see Job.
 */
class CoroWorker
{
public:
    /**
     * Job represents a job that accepts and returns continuation.
     */
    using Job = std::function<cc_t(cc_t &&)>;
    CoroWorker() = default;
    CoroWorker(CoroWorker &&rhs);
    CoroWorker(const CoroWorker &) = delete;
    CoroWorker &operator=(CoroWorker &&rhs);
    CoroWorker &operator=(const CoroWorker &rhs) = delete;
    ~CoroWorker() = default;

    /**
     * @brief register a Job to the worker.
     *
     * The Job may suspend itself by calling `cc_t.resume()`, and it will
     * be scheduled to resume later
     *
     * @param job the job to register. @see Job
     */
    void Register(Job job);
    /**
     * @brief worker starts to work.
     *
     * This invocation blocks until all the jobs have finished
     */
    void Serve();

private:
    std::queue<cc_t> works_;
};
}  // namespace symcoro
#endif
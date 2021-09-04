#ifndef PROTOTYPE_ACTION_RPG_THREADPOOL_HPP
#define PROTOTYPE_ACTION_RPG_THREADPOOL_HPP


#include <functional>
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>

#include "utils/NonCopyable.hpp"


namespace re {

    /**
     * @brief Job System manager class
     */
    class JobSystem : NonCopyable {
    public:
        using Job = std::function<void()>;

    public:
        static void setUp();

        static void stop();

        static void submit(Job job);

        static bool empty();

    private:
        JobSystem();

        static void waitJob();

    private:
        static bool done;
        static std::queue<Job> jobs;
        static std::mutex queueMutex;
        static std::vector<std::thread> pool;
        static std::mutex poolMutex;
        static std::condition_variable poolSignal;
    };

} // namespace


#endif //PROTOTYPE_ACTION_RPG_THREADPOOL_HPP

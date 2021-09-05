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
        /**
         * @brief Setup Thread with the num of core available - 1(main thread)
         */
        static void setUp();

        /**
         * @brief Joint threads
         */
        static void stop();

        /**
         * @brief Submit job to queue
         * @param job void function without parameters
         */
        static void submit(Job job);

        /**
         * @brief Check if jobs queue is empty
         */
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

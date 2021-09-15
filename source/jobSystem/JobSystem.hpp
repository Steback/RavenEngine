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
        friend class Base;

        JobSystem();

    public:
        using Job = std::function<void()>;

        ~JobSystem() override;

        static JobSystem* getInstance();

        /**
         * @brief Submit job to queue
         * @param job void function without parameters
         */
         void submit(Job job);

        /**
         * @brief Check if jobs queue is empty
         */
         bool empty();

    private:
         void waitJob();

    private:
        static JobSystem* singleton;
        bool done;
        std::queue<Job> jobs;
        std::mutex queueMutex;
        std::vector<std::thread> pool;
        std::mutex poolMutex;
        std::condition_variable poolSignal;
    };

} // namespace


#endif //PROTOTYPE_ACTION_RPG_THREADPOOL_HPP

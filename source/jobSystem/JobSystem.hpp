#ifndef PROTOTYPE_ACTION_RPG_THREADPOOL_HPP
#define PROTOTYPE_ACTION_RPG_THREADPOOL_HPP


#include <functional>
#include <thread>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <utility>

#include "utils/NonCopyable.hpp"


namespace re {

    class Base;

    namespace jobs {
        using Job = std::function<void()>;

        class JobSystem : NonCopyable {
            friend re::Base;

            JobSystem();

        public:

            ~JobSystem() override;

            static JobSystem* getInstance();

            void submit(Job job);

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

        inline void submit(Job job) {
            JobSystem::getInstance()->submit(std::move(job));
        }

        inline bool empty() {
            return JobSystem::getInstance()->empty();
        }

    } // namespace jobs

} // namespace re


#endif //PROTOTYPE_ACTION_RPG_THREADPOOL_HPP

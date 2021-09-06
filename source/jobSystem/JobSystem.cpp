#include "JobSystem.hpp"

#include "spdlog/spdlog.h"


namespace re {

    JobSystem::JobSystem() {
        done = false;

        const unsigned threadCount = std::thread::hardware_concurrency() - 1;
        for (int i = 0; i < threadCount; ++i)
            pool.emplace_back(&JobSystem::waitJob, this);
    }

    JobSystem::~JobSystem() {
        {
            std::unique_lock<std::mutex> lock(poolMutex);
            done = true;
        }
        poolSignal.notify_all();

        for (auto& thread : pool)
            thread.join();

        pool.clear();
    }

    void JobSystem::submit(JobSystem::Job job) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            jobs.push(std::move(job));
        }
        poolSignal.notify_one();
    }

    bool JobSystem::empty() {
        return jobs.empty();
    }

    void JobSystem::waitJob() {
        while (true) {
            Job job;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                poolSignal.wait(lock, [=, this]{return done || !jobs.empty();});

                if (done && jobs.empty()) break;

                job = std::move(jobs.front());
                jobs.pop();
            }
            job();
        }
    }

} // namespace re
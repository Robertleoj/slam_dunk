#pragma once
#include <spdlog/spdlog.h>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <thread>
#include <vector>

namespace slamd {

class RenderJobQueue {
   public:
    void enqueue(
        const std::function<void()>& job
    ) {
        std::scoped_lock lock(job_queue_mutex);

        jobs.push_back(job);
    }

    void execute_all() {
        std::vector<std::function<void()>> to_run;

        {
            std::scoped_lock lock(job_queue_mutex);
            to_run.swap(jobs);
        }

        for (auto& job : to_run) {
            job();
        }
    }

   private:
    std::vector<std::function<void()>> jobs;
    std::mutex job_queue_mutex;
};

class RenderQueueManager {
   public:
    static void ensure_current_thread_queue() {
        std::scoped_lock lock(map_mutex);

        auto current_id = std::this_thread::get_id();

        if (queues.find(current_id) != queues.end()) {
            return;
        }
        queues[current_id] = std::make_shared<RenderJobQueue>();
    }

    static std::optional<std::shared_ptr<RenderJobQueue>> get_queue(
        std::thread::id id
    ) {
        std::scoped_lock lock(map_mutex);

        auto it = queues.find(id);

        if (it != queues.end()) {
            return it->second;
        }

        return std::nullopt;
    }

    static std::optional<std::shared_ptr<RenderJobQueue>>
    get_current_thread_queue() {
        return get_queue(std::this_thread::get_id());
    }

   private:
    static inline std::map<std::thread::id, std::shared_ptr<RenderJobQueue>>
        queues;

    static inline std::mutex map_mutex;
};

}  // namespace slamd
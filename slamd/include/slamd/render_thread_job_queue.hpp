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
    void enqueue(const std::function<void()>& job);

    void execute_all();

   private:
    std::vector<std::function<void()>> jobs;
    std::mutex job_queue_mutex;
};

class RenderQueueManager {
   public:
    static void ensure_current_thread_queue();

    static void
    force_enqueue(std::thread::id thread_id, std::function<void()> job);

    static std::optional<std::shared_ptr<RenderJobQueue>> get_queue(
        std::thread::id id
    );

    static std::optional<std::shared_ptr<RenderJobQueue>>
    get_current_thread_queue();

   private:
    static inline std::map<std::thread::id, std::shared_ptr<RenderJobQueue>>
        queues;

    static inline std::mutex map_mutex;
};

}  // namespace slamd
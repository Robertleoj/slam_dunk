#include <slamd/render_thread_job_queue.hpp>

namespace slamd {

void RenderJobQueue::enqueue(
    const std::function<void()>& job
) {
    std::scoped_lock lock(job_queue_mutex);

    jobs.push_back(job);
}

void RenderJobQueue::execute_all() {
    std::vector<std::function<void()>> to_run;

    {
        std::scoped_lock lock(job_queue_mutex);
        to_run.swap(jobs);
    }

    for (auto& job : to_run) {
        job();
    }
}

void RenderQueueManager::ensure_current_thread_queue() {
    std::scoped_lock lock(map_mutex);

    auto current_id = std::this_thread::get_id();

    if (queues.find(current_id) != queues.end()) {
        return;
    }
    queues[current_id] = std::make_shared<RenderJobQueue>();
}

void RenderQueueManager::force_enqueue(
    std::thread::id thread_id,
    std::function<void()> job
) {
    auto render_queue = get_queue(thread_id);

    if (!render_queue.has_value()) {
        throw std::runtime_error("No render queue available!");
    }

    render_queue.value()->enqueue(job);
}

std::optional<std::shared_ptr<RenderJobQueue>> RenderQueueManager::get_queue(
    std::thread::id id
) {
    std::scoped_lock lock(map_mutex);

    auto it = queues.find(id);

    if (it != queues.end()) {
        return it->second;
    }

    return std::nullopt;
}

std::optional<std::shared_ptr<RenderJobQueue>>
RenderQueueManager::get_current_thread_queue() {
    return get_queue(std::this_thread::get_id());
}

}  // namespace slamd

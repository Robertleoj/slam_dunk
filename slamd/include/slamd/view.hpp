#pragma once
#include <flatb/visualizer_generated.h>
#include <atomic>
#include <memory>

namespace slamd {

class View : std::enable_shared_from_this<View> {
   public:
    View(uint64_t tree_id, slamd::flatb::ViewType view_type);
    const uint64_t tree_id;
    slamd::flatb::ViewType view_type;

    const uint64_t id;

   private:
    static std::atomic<uint64_t> id_counter;
};

}  // namespace slamd
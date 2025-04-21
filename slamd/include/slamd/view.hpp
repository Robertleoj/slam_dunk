#pragma once
#include <visualizer_generated.h>

namespace slamd {

struct View {
    uint64_t tree_id;
    slamd::flatb::ViewType view_type;
};

}  // namespace slamd
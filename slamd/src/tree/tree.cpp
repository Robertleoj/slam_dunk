#include <spdlog/spdlog.h>
#include <slamd/tree/tree.hpp>

namespace slamd {

std::shared_ptr<Scene> scene() {
    return std::make_shared<Scene>();
}

}  // namespace slamd
#include <slamd/object_tracker.hpp>

namespace slamd {
namespace _global {

ObjectTracker<_view::View> views;
ObjectTracker<_tree::Node> nodes;

}  // namespace _global
}  // namespace slamd

#include <slamd/object_tracker.hpp>

namespace slamd {
namespace _global {

ObjectTracker<_id::ViewID, _view::View> views;
ObjectTracker<_id::NodeID, _tree::Node> nodes;

}  // namespace _global
}  // namespace slamd

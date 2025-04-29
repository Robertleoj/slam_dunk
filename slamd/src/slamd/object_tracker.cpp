#include <slamd/object_tracker.hpp>

namespace slamd {
namespace _tracker {

ObjectTracker& ObjectTracker::instance() {
    static ObjectTracker instance;
    return instance;
}

ObjectTracker::ObjectTracker() {}

}  // namespace _tracker

namespace _global {

GlobalObjectMap<_vis::Visualizer> visualizers;
GlobalObjectMap<_tree::Tree> trees;
GlobalObjectMap<_view::View> views;
GlobalObjectMap<_geom::Geometry> geometries;

}  // namespace _global
}  // namespace slamd

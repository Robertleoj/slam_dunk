#include <slamd/global_object_map.hpp>

namespace slamd {
namespace _global {

GlobalObjectMap<Visualizer> visualizers;
GlobalObjectMap<_tree::Tree> trees;
GlobalObjectMap<View> views;
GlobalObjectMap<_geom::Geometry> geometries;

}  // namespace _global
}  // namespace slamd

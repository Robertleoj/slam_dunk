#pragma once
#include <slamd/arcball.hpp>
#include <slamd/camera.hpp>
#include <slamd/frame_buffer.hpp>
#include <slamd/frame_timer.hpp>
#include <slamd/scene_objects/arcball_indicator.hpp>
#include <slamd/scene_objects/xy_grid.hpp>
#include <slamd/tree.hpp>
#include <thread>

namespace slamd {

class Scene {
   public:
    Tree tree;

   public:
    Scene();
    void render_to_imgui();

   private:
    void handle_input();
    void render_to_frame_buffer();
    FrameBuffer frame_buffer;
    Arcball arcball;
    Camera camera;
    FrameTimer frame_timer;
    GridXYPlane xy_grid;
    ArcballIndicator arcball_indicator;
};

}  // namespace slamd
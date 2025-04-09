#pragma once
#include <slamd/arcball.hpp>
#include <slamd/camera.hpp>
#include <slamd/frame_buffer.hpp>
#include <slamd/frame_timer.hpp>
#include <slamd/geometry/arcball_indicator.hpp>
#include <slamd/geometry/xy_grid.hpp>
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
    void handle_translation_input();
    void handle_mouse_input();

    void render_to_frame_buffer();
    FrameBuffer frame_buffer;
    Arcball arcball;
    Camera camera;
    FrameTimer frame_timer;
    geometry::GridXYPlane xy_grid;
    geometry::ArcballIndicator arcball_indicator;
};

std::shared_ptr<Scene> scene();

}  // namespace slamd
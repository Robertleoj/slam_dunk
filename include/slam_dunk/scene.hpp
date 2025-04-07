#pragma once
#include <slam_dunk/arcball.hpp>
#include <slam_dunk/camera.hpp>
#include <slam_dunk/frame_buffer.hpp>
#include <slam_dunk/tree.hpp>
#include <thread>

namespace sdunk {

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
};

}  // namespace sdunk
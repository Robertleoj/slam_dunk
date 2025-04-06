#pragma once
#include <slam_dunk/frame_buffer.hpp>
#include <slam_dunk/tree.hpp>
#include <thread>
#include <slam_dunk/camera.hpp>

namespace sdunk {

class Scene {
   public:
    Tree tree;


   public:
    Scene();
    void render_to_imgui();

   private:
    void render_to_frame_buffer();
    FrameBuffer frame_buffer;
    Camera camera;
};

}  // namespace sdunk
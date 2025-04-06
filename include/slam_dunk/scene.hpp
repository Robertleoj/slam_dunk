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
    void render_to_frame_buffer();
    FrameBuffer frame_buffer;
};

}  // namespace sdunk
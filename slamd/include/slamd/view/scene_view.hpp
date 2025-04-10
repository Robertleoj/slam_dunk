#pragma once
#include <slamd/arcball.hpp>
#include <slamd/camera.hpp>
#include <slamd/frame_buffer.hpp>
#include <slamd/frame_timer.hpp>
#include <slamd/geom/arcball_indicator.hpp>
#include <slamd/geom/xy_grid.hpp>
#include <slamd/tree/tree.hpp>
#include <slamd/view/view.hpp>
#include <thread>

namespace slamd {
class SceneView : public View {
   public:
    std::shared_ptr<Scene> scene;

   private:
    FrameBuffer frame_buffer;
    Arcball arcball;
    Camera camera;
    FrameTimer frame_timer;
    _geom::GridXYPlane xy_grid;
    _geom::ArcballIndicator arcball_indicator;

   public:
    SceneView(std::shared_ptr<Scene> scene);
    void render_to_imgui() override;

   private:
    void handle_input();
    void handle_translation_input();
    void handle_mouse_input();

    void render_to_frame_buffer();
};

}  // namespace slamd
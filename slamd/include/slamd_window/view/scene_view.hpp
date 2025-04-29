#pragma once
#include <slamd_window/arcball.hpp>
#include <slamd_window/camera.hpp>
#include <slamd_window/frame_buffer.hpp>
#include <slamd_window/frame_timer.hpp>
#include <slamd_window/geom/arcball_indicator.hpp>
#include <slamd_window/geom/xy_grid.hpp>
#include <slamd_window/tree/tree.hpp>
#include <slamd_window/view/view.hpp>

namespace slamdw {
class SceneView : public View {
   public:
    std::shared_ptr<Tree> tree;

   private:
    FrameBuffer frame_buffer;
    Arcball arcball;
    Camera camera;
    FrameTimer frame_timer;
    _geom::GridXYPlane xy_grid;
    _geom::ArcballIndicator arcball_indicator;

   public:
    SceneView(std::shared_ptr<Tree> tree);
    void render_to_imgui() override;

   private:
    void handle_input();
    void handle_translation_input();
    void handle_mouse_input();

    void render_to_frame_buffer();
};

}  // namespace slamdw
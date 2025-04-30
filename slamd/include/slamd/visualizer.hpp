#pragma once
#include <asio.hpp>
#include <memory>
#include <mutex>
#include <slamd/net/client_set.hpp>
#include <slamd/tree/tree.hpp>
#include <slamd/view.hpp>
#include <slamd_common/id.hpp>
#include <stop_token>
#include <thread>

namespace slamd {
namespace _id {}  // namespace _id

namespace _view {
class View;
}

namespace _tree {
class Tree;
}
class Scene;
class Canvas;

namespace _vis {

class Visualizer : public std::enable_shared_from_this<Visualizer> {
   public:
    Visualizer(std::string name);
    ~Visualizer();
    void add_scene(std::string name, std::shared_ptr<Scene> scene);
    void add_canvas(std::string name, std::shared_ptr<Canvas> canvas);
    void hang_forever();

    void broadcast(std::shared_ptr<std::vector<uint8_t>> message_buffer);

   private:
    void server_job(std::stop_token& stop_token);
    std::vector<uint8_t> get_state();
    flatbuffers::Offset<
        flatbuffers::Vector<flatbuffers::Offset<flatb::Geometry>>>
    get_geometries_fb(flatbuffers::FlatBufferBuilder& builder);

   public:
    _id::VisualizerID id;

   private:
    std::string name;
    std::jthread server_thread;

    std::mutex view_map_mutex;

    std::map<std::string, std::shared_ptr<_view::View>> view_name_to_view;
    std::map<_id::TreeID, std::shared_ptr<_tree::Tree>> trees;
    std::shared_ptr<_net::ClientSet> client_set;
};

}  // namespace _vis

using VisualizerPtr = std::shared_ptr<_vis::Visualizer>;

VisualizerPtr visualizer(std::string name);

}  // namespace slamd
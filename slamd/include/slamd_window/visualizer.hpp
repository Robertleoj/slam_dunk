#pragma once
#include <cstddef>
#include <filesystem>
#include <memory>
#include <mutex>
#include <asio.hpp>
#include <slamd/connection.hpp>
#include <slamd/glfw.hpp>
#include <slamd/view/canvas_view.hpp>
#include <slamd/view/scene_view.hpp>
#include <stop_token>
#include <thread>
#include <list>
#include <vector>

namespace slamd {

namespace fs = std::filesystem;

class Visualizer {
   public:
    Visualizer(std::string name);
    ~Visualizer();
    void add_scene(std::string name, std::shared_ptr<Scene> scene);
    void add_canvas(std::string name, std::shared_ptr<Canvas> canvas);

   private:
    void server_job(std::stop_token& stop_token);
    void connection_handler(asio::ip::tcp::socket socket);

   private:
    std::string name;
    std::jthread server_thread;

    std::mutex view_map_mutex;
    std::map<std::string, std::shared_ptr<View>> view_map;
    std::list<Connection> open_connections;
};

}  // namespace slamd
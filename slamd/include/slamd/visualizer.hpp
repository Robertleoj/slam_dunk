#pragma once
#include <asio.hpp>
#include <cstddef>
#include <filesystem>
#include <list>
#include <memory>
#include <mutex>
#include <slamd/connection.hpp>
#include <slamd/tree/tree.hpp>
#include <slamd/view.hpp>
#include <stop_token>
#include <thread>
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
    std::vector<uint8_t> get_state();

   private:
    std::string name;
    std::jthread server_thread;

    std::mutex view_map_mutex;

    std::map<std::string, View> view_name_to_view;
    std::map<uint64_t, std::shared_ptr<_tree::Tree>> trees;

    std::list<Connection> open_connections;
};

}  // namespace slamd
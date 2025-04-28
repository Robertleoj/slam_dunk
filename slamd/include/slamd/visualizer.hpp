#pragma once
#include <asio.hpp>
#include <slamd/net/clients.hpp>
#include <memory>
#include <mutex>
#include <slamd/tree/tree.hpp>
#include <slamd/view.hpp>
#include <stop_token>
#include <thread>

namespace slamd {

class Visualizer {
   public:
    Visualizer(std::string name);
    ~Visualizer();
    void add_scene(std::string name, std::shared_ptr<Scene> scene);
    void add_canvas(std::string name, std::shared_ptr<Canvas> canvas);
    void hang_forever();

   private:
    void server_job(std::stop_token& stop_token);
    std::vector<uint8_t> get_state();

   private:
    std::string name;
    std::jthread server_thread;

    std::mutex view_map_mutex;

    std::map<std::string, View> view_name_to_view;
    std::map<uint64_t, std::shared_ptr<_tree::Tree>> trees;
    _net::Clients clients;
};

}  // namespace slamd
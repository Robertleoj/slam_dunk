#pragma once

#include <map>
#include <memory>
#include <slamd_window/connection.hpp>
#include <slamd_window/tree/tree.hpp>
#include <slamd_window/view/view.hpp>
#include <string>

namespace slamdw {

class StateManager {
   public:
    StateManager();

    void try_connect(std::string ip = "127.0.0.1", ushort port = 555);

    void apply_updates();

   private:
    void handle_initial_state(const slamd::flatb::InitialState* initial_state);

    // fs::path layout_path();

   public:
    bool loaded = false;
    std::string name;
    std::map<std::string, std::unique_ptr<View>> views;
    std::map<uint64_t, std::shared_ptr<Tree>> trees;

   private:
    std::optional<Connection> connection;
};
}  // namespace slamdw
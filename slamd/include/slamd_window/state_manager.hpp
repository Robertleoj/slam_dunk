#pragma once

#include <map>
#include <memory>
#include <slamd_common/id.hpp>
#include <slamd_window/connection.hpp>
#include <slamd_window/tree/tree.hpp>
#include <slamd_window/view/view.hpp>
#include <string>

namespace slamd {

class StateManager {
   public:
    StateManager();

    void try_connect(std::string ip = "127.0.0.1", ushort port = 555);

    void apply_updates();

   private:
    void handle_initial_state(const slamd::flatb::InitialState* initial_state);
    void handle_set_transform(const slamd::flatb::SetTransform* set_transform_fb
    );
    void handle_set_object(const slamd::flatb::SetObject* set_object_fb);
    void handle_add_geometry(const slamd::flatb::AddGeometry* add_geometry_fb);
    void handle_remove_geometry(
        const slamd::flatb::RemoveGeometry* remove_geometry_fb
    );

    // fs::path layout_path();

   public:
    bool loaded = false;
    std::string name;
    std::map<std::string, std::unique_ptr<View>> views;
    std::map<_id::TreeID, std::shared_ptr<Tree>> trees;
    std::map<_id::GeometryID, std::shared_ptr<_geom::Geometry>> geometries;

   private:
    std::optional<Connection> connection;
};
}  // namespace slamd
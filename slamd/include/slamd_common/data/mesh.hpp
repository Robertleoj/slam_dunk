#pragma once

#include <flatb/primitives_generated.h>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace slamd {
namespace data {

class MeshData {
   public:
    MeshData();
    MeshData(const MeshData& other);
    MeshData(MeshData&& other);
    MeshData(
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec3>& colors,
        const std::vector<uint32_t>& triangle_indices,
        const std::vector<glm::vec3>& normals
    );

    void recompute_normals();

    void add_vertex(
        const glm::vec3& pos,
        const glm::vec3& color,
        const glm::vec3& normal
    );

    flatbuffers::Offset<slamd::flatb::MeshData> serialize(
        flatbuffers::FlatBufferBuilder& builder
    );

    static MeshData deserialize(const flatb::MeshData* meshdata_fb);

   public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> colors;
    std::vector<uint32_t> triangle_indices;
    std::vector<glm::vec3> normals;
};

class MeshDataBuilder {
   public:
    MeshDataBuilder();

    // Position setters
    MeshDataBuilder& set_positions(const std::vector<glm::vec3>& positions);
    MeshDataBuilder& set_positions(std::vector<glm::vec3>&& positions);  // move

    // Color setters
    MeshDataBuilder& set_colors(const std::vector<glm::vec3>& colors);
    MeshDataBuilder& set_colors(std::vector<glm::vec3>&& colors);  // move
    MeshDataBuilder& set_colors(const glm::vec3& color);

    // Normal setters
    MeshDataBuilder& set_normals(const std::vector<glm::vec3>& normals);
    MeshDataBuilder& set_normals(std::vector<glm::vec3>&& normals);  // move
    MeshDataBuilder& set_normals(const glm::vec3& normal);
    MeshDataBuilder& compute_normals();

    // Index setters
    MeshDataBuilder& set_indices(const std::vector<uint32_t>& indices);
    MeshDataBuilder& set_indices(std::vector<uint32_t>&& indices);  // move

    MeshData build();

   public:
    std::optional<std::vector<glm::vec3>> positions;
    std::optional<std::vector<glm::vec3>> colors;
    std::optional<std::vector<uint32_t>> triangle_indices;
    std::optional<std::vector<glm::vec3>> normals;
};

}  // namespace data

}  // namespace slamd
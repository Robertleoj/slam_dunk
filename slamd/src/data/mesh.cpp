#include <slamd/data/mesh.hpp>
#include <slamd/geom/utils.hpp>
#include <stdexcept>

namespace slamd {
namespace data {

MeshData::MeshData() = default;

MeshData::MeshData(
    const std::vector<glm::vec3>& positions,
    const std::vector<glm::vec3>& colors,
    const std::vector<uint32_t>& triangle_indices,
    const std::vector<glm::vec3>& normals
)
    : positions(positions),
      colors(colors),
      triangle_indices(triangle_indices),
      normals(normals) {}

void MeshData::add_vertex(
    const glm::vec3& pos,
    const glm::vec3& color,
    const glm::vec3& normal
) {
    positions.push_back(pos);
    colors.push_back(color);
    normals.push_back(normal);
}

void MeshData::recompute_normals() {
    this->normals =
        _geom::compute_vertex_normals(this->positions, this->triangle_indices);
}
// ==== MeshDataBuilder ====

MeshDataBuilder::MeshDataBuilder() = default;

// Position setters
MeshDataBuilder& MeshDataBuilder::set_positions(
    const std::vector<glm::vec3>& pos
) {
    positions = pos;
    return *this;
}

MeshDataBuilder& MeshDataBuilder::set_positions(
    std::vector<glm::vec3>&& pos
) {
    positions = std::move(pos);
    return *this;
}

// Color setters
MeshDataBuilder& MeshDataBuilder::set_colors(
    const std::vector<glm::vec3>& col
) {
    colors = col;
    return *this;
}

MeshDataBuilder& MeshDataBuilder::set_colors(
    std::vector<glm::vec3>&& col
) {
    colors = std::move(col);
    return *this;
}

MeshDataBuilder& MeshDataBuilder::set_colors(
    const glm::vec3& color
) {
    if (!positions.has_value()) {
        throw std::runtime_error(
            "Set positions before using uniform color setter"
        );
    }

    std::vector<glm::vec3> uniform_colors(positions->size(), color);
    colors = std::move(uniform_colors);
    return *this;
}

// Normal setters
MeshDataBuilder& MeshDataBuilder::set_normals(
    const std::vector<glm::vec3>& norm
) {
    normals = norm;
    return *this;
}

MeshDataBuilder& MeshDataBuilder::set_normals(
    std::vector<glm::vec3>&& norm
) {
    normals = std::move(norm);
    return *this;
}

MeshDataBuilder& MeshDataBuilder::set_normals(
    const glm::vec3& normal
) {
    if (!this->positions.has_value()) {
        throw std::runtime_error(
            "Set positions before using uniform color setter"
        );
    }

    std::vector<glm::vec3> uniform_normals(positions->size(), normal);
    this->normals = std::move(uniform_normals);
    return *this;
}

MeshDataBuilder& MeshDataBuilder::compute_normals() {
    if (!this->positions.has_value() || !this->triangle_indices.has_value()) {
        throw std::runtime_error(
            "Have to provide positions and indices before computing normals"
        );
    }

    this->normals = _geom::compute_vertex_normals(
        this->positions.value(),
        this->triangle_indices.value()
    );

    return *this;
}

// Index setters
MeshDataBuilder& MeshDataBuilder::set_indices(
    const std::vector<uint32_t>& idx
) {
    triangle_indices = idx;
    return *this;
}

MeshDataBuilder& MeshDataBuilder::set_indices(
    std::vector<uint32_t>&& idx
) {
    triangle_indices = std::move(idx);
    return *this;
}

// Final build
MeshData MeshDataBuilder::build() {
    data::MeshData data;

    if (!positions.has_value() || this->positions.value().empty()) {
        throw std::runtime_error("Mesh must have positions");
    }

    data.positions = std::move(this->positions.value());

    if (!this->colors.has_value() || this->colors.value().empty()) {
        throw std::runtime_error("Color(s) not specified");
    }

    data.colors = std::move(this->colors.value());

    if (!this->triangle_indices.has_value() ||
        this->triangle_indices.value().empty()) {
        throw std::runtime_error("Mesh needs indices");
    }

    data.triangle_indices = std::move(this->triangle_indices.value());

    if (!normals.has_value()) {
        this->compute_normals();
    }

    data.normals = std::move(this->normals.value());

    return data;
}

}  // namespace data
}  // namespace slamd
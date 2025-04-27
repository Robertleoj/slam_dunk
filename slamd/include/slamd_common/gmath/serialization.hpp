#pragma once

#include <flatb/primitives_generated.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slamd {
namespace gmath {

// mat4
inline slamd::flatb::Mat4 serialize(
    glm::mat4 mat
) {
    slamd::flatb::Mat4 mat4_fb;
    std::memcpy(
        (void*)mat4_fb.data()->data(),
        glm::value_ptr(mat),
        sizeof(float) * 16
    );
    return mat4_fb;
}

inline glm::mat4 deserialize(
    const slamd::flatb::Mat4* mat4_fb
) {
    glm::mat4 mat;
    std::memcpy(
        glm::value_ptr(mat),
        mat4_fb->data()->data(),
        sizeof(float) * 16
    );
    return mat;
}

inline flatbuffers::Offset<flatbuffers::Vector<const slamd::flatb::Mat4*>>
serialize_vector(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<glm::mat4>& elements_in
) {
    std::vector<slamd::flatb::Mat4> elements_out;
    elements_out.reserve(elements_in.size());

    for (const auto& value : elements_in) {
        elements_out.push_back(serialize(value));
    }

    return builder.CreateVectorOfStructs(elements_out);
}

inline std::vector<glm::mat4> deserialize_vector(
    const flatbuffers::Vector<const slamd::flatb::Mat4*>* vec_fb
) {
    std::vector<glm::mat4> result;
    result.reserve(vec_fb->size());
    for (auto fb_elem : *vec_fb) {
        result.push_back(deserialize(fb_elem));
    }
    return result;
}

// vec2
inline slamd::flatb::Vec2 serialize(
    glm::vec2 vec
) {
    return {vec.x, vec.y};
}

inline glm::vec2 deserialize(
    const slamd::flatb::Vec2* vec2_fb
) {
    return glm::vec2(vec2_fb->x(), vec2_fb->y());
}

inline flatbuffers::Offset<flatbuffers::Vector<const slamd::flatb::Vec2*>>
serialize_vector(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<glm::vec2>& elements_in
) {
    std::vector<slamd::flatb::Vec2> elements_out;
    elements_out.reserve(elements_in.size());

    for (const auto& value : elements_in) {
        elements_out.push_back(serialize(value));
    }

    return builder.CreateVectorOfStructs(elements_out);
}

inline std::vector<glm::vec2> deserialize_vector(
    const flatbuffers::Vector<const slamd::flatb::Vec2*>* vec_fb
) {
    std::vector<glm::vec2> result;
    result.reserve(vec_fb->size());
    for (auto fb_elem : *vec_fb) {
        result.push_back(deserialize(fb_elem));
    }
    return result;
}

// vec3
inline slamd::flatb::Vec3 serialize(
    glm::vec3 vec
) {
    return {vec.x, vec.y, vec.z};
}

inline glm::vec3 deserialize(
    const slamd::flatb::Vec3* vec3_fb
) {
    return glm::vec3(vec3_fb->x(), vec3_fb->y(), vec3_fb->z());
}

inline flatbuffers::Offset<flatbuffers::Vector<const slamd::flatb::Vec3*>>
serialize_vector(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<glm::vec3>& elements_in
) {
    std::vector<slamd::flatb::Vec3> elements_out;
    elements_out.reserve(elements_in.size());

    for (const auto& value : elements_in) {
        elements_out.push_back(serialize(value));
    }

    return builder.CreateVectorOfStructs(elements_out);
}

inline std::vector<glm::vec3> deserialize_vector(
    const flatbuffers::Vector<const slamd::flatb::Vec3*>* vec_fb
) {
    std::vector<glm::vec3> result;
    result.reserve(vec_fb->size());
    for (auto fb_elem : *vec_fb) {
        result.push_back(deserialize(fb_elem));
    }
    return result;
}

// float
inline flatbuffers::Offset<flatbuffers::Vector<float>> serialize_vector(
    flatbuffers::FlatBufferBuilder& builder,
    const std::vector<float>& elements_in
) {
    return builder.CreateVector(elements_in);
}

inline std::vector<float> deserialize_vector(
    const flatbuffers::Vector<float>* vec_fb
) {
    std::vector<float> result;
    result.reserve(vec_fb->size());
    for (auto value : *vec_fb) {
        result.push_back(value);
    }
    return result;
}

}  // namespace gmath
}  // namespace slamd

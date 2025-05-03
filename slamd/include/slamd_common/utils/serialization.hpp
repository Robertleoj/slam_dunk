#pragma once

#include <flatbuffers/flatbuffers.h>
#include <cstdint>
#include <memory>
#include <vector>

namespace slamd {
namespace _utils {

inline std::shared_ptr<std::vector<uint8_t>> builder_buffer(
    flatbuffers::FlatBufferBuilder& builder
) {
    uint8_t* ptr = builder.GetBufferPointer();
    size_t size = builder.GetSize();

    return std::make_shared<std::vector<uint8_t>>(ptr, ptr + size);
}

}  // namespace _utils
}  // namespace slamd
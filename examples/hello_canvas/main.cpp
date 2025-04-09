#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cstdint>
#include <filesystem>
#include <format>
#include <iostream>
#include <slamd/slamd.hpp>

namespace fs = std::filesystem;

const fs::path image_path = fs::path(__FILE__).parent_path() / "image.png";

slamd::data::Image read_image() {
    int width, height, channels;
    unsigned char* data_ptr =
        stbi_load(image_path.string().c_str(), &width, &height, &channels, 0);

    size_t num_bytes = sizeof(uint8_t) * height * width * channels;

    // clang-format off
    std::cout 
        << "Loaded image:\n"
        << "\twidth: " << width << "\n"
        << "\theight: " << height << "\n"
        << "\tchannels: " << channels << "\n"
        << "\ttotal bytes: " << num_bytes
        << std::endl;
    // clang-format on

    std::vector<uint8_t> data(data_ptr, data_ptr + num_bytes);

    return slamd::data::Image(std::move(data), width, height, channels);
}

int main() {
    slamd::Window window(1000, 1000);

    auto canvas = slamd::canvas();

    window.add_canvas("canvas", canvas);

    slamd::data::Image image = read_image();

    window.wait_for_close();
}
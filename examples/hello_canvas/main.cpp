#define STB_IMAGE_IMPLEMENTATION
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <cstdint>
#include <filesystem>
#include <format>
#include <iostream>
#include <random>
#include <slamd/slamd.hpp>
#include <stdexcept>

namespace fs = std::filesystem;

std::random_device rd;
std::mt19937 gen(rd());

const fs::path image_path = fs::path(__FILE__)
                                .parent_path()  // hello canvas
                                .parent_path()  // examples
                                .parent_path()  // repo root
                            / "images" / "logo.png";

float random_float(
    float start,
    float end
) {
    std::uniform_real_distribution<float> dist(start, end);
    return dist(gen);
}

slamd::data::Image read_image() {
    int width, height, channels;
    unsigned char* data_ptr =
        stbi_load(image_path.string().c_str(), &width, &height, &channels, 0);

    if (data_ptr == nullptr) {
        throw std::runtime_error(
            std::format("Failed to load image at", image_path.string())
        );
    }

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

    stbi_image_free(data_ptr);

    return slamd::data::Image(std::move(data), width, height, channels);
}

auto random_points() {
    std::vector<glm::vec2> pos;
    std::vector<float> rad;
    std::vector<glm::vec3> col;

    for (size_t i = 0; i < 500; i++) {
        pos.emplace_back(
            random_float(0.0f, 1024.0f),
            random_float(0.0f, 630.0f)
        );

        rad.push_back(random_float(5.0f, 10.0f));

        col.emplace_back(
            random_float(0.0f, 1.0f),
            random_float(0.0f, 1.0f),
            random_float(0.0f, 1.0f)
        );
    }

    return slamd::geom2d::points_2d(pos, col, rad);
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    slamd::Window window("hello_canvas", 1000, 1000);

    auto canvas = slamd::canvas();

    window.add_canvas("canvas", canvas);

    slamd::data::Image image = read_image();

    canvas->set_object("/image", slamd::geom2d::image(std::move(image)));

    canvas->set_object("/points", random_points());

    window.wait_for_close();
}
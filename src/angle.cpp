#include <glm/glm.hpp>
#include <slam_dunk/angle.hpp>

namespace slamd {

Angle::Angle(
    float radians
)
    : radians(radians) {}

Angle Angle::rad(
    float radians
) {
    return Angle(radians);
}

Angle Angle::deg(
    float degrees
) {
    float radians = glm::radians(degrees);
    return Angle(radians);
}

float Angle::rad() const {
    return this->radians;
}

float Angle::deg() const {
    return glm::degrees(this->radians);
}

Angle Angle::operator+(
    const Angle& other
) const {
    return Angle(this->radians + other.radians);
}

Angle Angle::clamp(
    const Angle& min,
    const Angle& max
) const {
    float clipped_radians = glm::clamp(this->rad(), min.rad(), max.rad());

    return rad(clipped_radians);
}
Angle Angle::operator-() const {
    return rad(-this->rad());
}

}  // namespace slamd
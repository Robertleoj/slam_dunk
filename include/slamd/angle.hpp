#pragma once

namespace slamd {
class Angle {
   public:
    Angle(float radians);

    float rad() const;
    float deg() const;
    Angle clamp(const Angle& min, const Angle& max) const;
    Angle operator+(const Angle& other) const;

    static Angle rad(float radians);
    static Angle deg(float degrees);

    Angle operator-() const;

   private:
    float radians;
};

}  // namespace slamd
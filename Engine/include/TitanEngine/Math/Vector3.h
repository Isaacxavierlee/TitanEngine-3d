#pragma once
#include <cmath>

namespace Titan {

    struct Vector3 {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float v) : x(v), y(v), z(v) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        float Dot(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        Vector3 Cross(const Vector3& other) const {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }

        float Length() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        Vector3 Normalized() const {
            float len = Length();
            if (len > 0.0001f) return *this * (1.0f / len);
            return Vector3(0, 0, 0);
        }

        static Vector3 Zero() { return Vector3(0, 0, 0); }
        static Vector3 Up() { return Vector3(0, 1, 0); }
        static Vector3 Right() { return Vector3(1, 0, 0); }
        static Vector3 Forward() { return Vector3(0, 0, 1); }
    };

}
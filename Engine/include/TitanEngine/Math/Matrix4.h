#pragma once
#include "Vector3.h"
#include <cstring>
#include <cmath>

namespace Titan {

    struct Matrix4 {
        float data[4][4];

        // Identity matrix
        Matrix4() {
            memset(data, 0, sizeof(data));
            data[0][0] = data[1][1] = data[2][2] = data[3][3] = 1.0f;
        }

        // Access operators
        float* operator[](int row) { return data[row]; }
        const float* operator[](int row) const { return data[row]; }

        // Matrix multiplication
        Matrix4 operator*(const Matrix4& other) const {
            Matrix4 result;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    result.data[i][j] = 0;
                    for (int k = 0; k < 4; k++) {
                        result.data[i][j] += data[i][k] * other.data[k][j];
                    }
                }
            }
            return result;
        }

        // Transform a point (includes translation)
        Vector3 TransformPoint(const Vector3& point) const {
            return Vector3(
                data[0][0] * point.x + data[0][1] * point.y + data[0][2] * point.z + data[0][3],
                data[1][0] * point.x + data[1][1] * point.y + data[1][2] * point.z + data[1][3],
                data[2][0] * point.x + data[2][1] * point.y + data[2][2] * point.z + data[2][3]
            );
        }

        // Transform a direction (ignores translation)
        Vector3 TransformDirection(const Vector3& dir) const {
            return Vector3(
                data[0][0] * dir.x + data[0][1] * dir.y + data[0][2] * dir.z,
                data[1][0] * dir.x + data[1][1] * dir.y + data[1][2] * dir.z,
                data[2][0] * dir.x + data[2][1] * dir.y + data[2][2] * dir.z
            );
        }

        // Static factory methods
        static Matrix4 Identity() { return Matrix4(); }

        static Matrix4 Translation(const Vector3& translation) {
            Matrix4 result;
            result.data[0][3] = translation.x;
            result.data[1][3] = translation.y;
            result.data[2][3] = translation.z;
            return result;
        }

        static Matrix4 Scale(const Vector3& scale) {
            Matrix4 result;
            result.data[0][0] = scale.x;
            result.data[1][1] = scale.y;
            result.data[2][2] = scale.z;
            return result;
        }

        static Matrix4 RotationX(float angle) {
            float c = cos(angle);
            float s = sin(angle);
            Matrix4 result;
            result.data[1][1] = c;
            result.data[1][2] = -s;
            result.data[2][1] = s;
            result.data[2][2] = c;
            return result;
        }

        static Matrix4 RotationY(float angle) {
            float c = cos(angle);
            float s = sin(angle);
            Matrix4 result;
            result.data[0][0] = c;
            result.data[0][2] = s;
            result.data[2][0] = -s;
            result.data[2][2] = c;
            return result;
        }

        static Matrix4 RotationZ(float angle) {
            float c = cos(angle);
            float s = sin(angle);
            Matrix4 result;
            result.data[0][0] = c;
            result.data[0][1] = -s;
            result.data[1][0] = s;
            result.data[1][1] = c;
            return result;
        }

        static Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
            Vector3 forward = (target - eye).Normalized();
            Vector3 right = up.Cross(forward).Normalized();
            Vector3 trueUp = forward.Cross(right);

            Matrix4 result;
            result[0][0] = right.x;   result[0][1] = right.y;   result[0][2] = right.z;   result[0][3] = -eye.Dot(right);
            result[1][0] = trueUp.x;  result[1][1] = trueUp.y;  result[1][2] = trueUp.z;  result[1][3] = -eye.Dot(trueUp);
            result[2][0] = -forward.x; result[2][1] = -forward.y; result[2][2] = -forward.z; result[2][3] = eye.Dot(forward);
            result[3][0] = 0;          result[3][1] = 0;          result[3][2] = 0;          result[3][3] = 1;
            return result;
        }

        static Matrix4 Perspective(float fov, float aspect, float near, float far) {
            float tanHalfFov = tan(fov * 0.5f);
            Matrix4 result;
            memset(result.data, 0, sizeof(result.data));
            result[0][0] = 1.0f / (aspect * tanHalfFov);
            result[1][1] = 1.0f / tanHalfFov;
            result[2][2] = -(far + near) / (far - near);
            result[2][3] = -(2.0f * far * near) / (far - near);
            result[3][2] = -1.0f;
            return result;
        }

        const float* Ptr() const { return &data[0][0]; }
    };

} // namespace Titan
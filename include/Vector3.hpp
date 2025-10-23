
#pragma once
#include <cmath>

namespace ParteeEngine {
    
    struct Vector3 {
        float x, y, z;
        
        Vector3(float x_ = 0, float y_ = 0, float z_ = 0) : x(x_), y(y_), z(z_) {}
        
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        
        Vector3 operator*(float scalar) const {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector3& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }
        
        float dot(const Vector3& other) const {
            return x * other.x + y * other.y + z * other.z;
        }
        
        Vector3 cross(const Vector3& other) const {
            return Vector3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
        }
        
        float length() const {
            return sqrt(x * x + y * y + z * z);
        }
        
        Vector3 normalize() const {
            float len = length();
            if (len > 0.0f) {
                return Vector3(x / len, y / len, z / len);
            }
            return Vector3(0, 0, 0);
        }
    };
}

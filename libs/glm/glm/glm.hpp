#pragma once

// Basic GLM vector and matrix types
struct vec3 {
    float x, y, z;
    vec3() : x(0), y(0), z(0) {}
    vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    vec3(float val) : x(val), y(val), z(val) {}
    
    vec3 operator+(const vec3& other) const { return vec3(x + other.x, y + other.y, z + other.z); }
    vec3 operator-(const vec3& other) const { return vec3(x - other.x, y - other.y, z - other.z); }
    vec3 operator*(float val) const { return vec3(x * val, y * val, z * val); }
    vec3& operator+=(const vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    vec3& operator*=(const vec3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
};

struct vec2 {
    float x, y;
    vec2() : x(0), y(0) {}
    vec2(float x_, float y_) : x(x_), y(y_) {}
    vec2(float val) : x(val), y(val) {}
};

struct mat4 {
    float data[16];
    
    mat4() {
        // Initialize as identity matrix
        for (int i = 0; i < 16; i++) data[i] = 0.0f;
        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }
    
    mat4(float val) {
        for (int i = 0; i < 16; i++) data[i] = 0.0f;
        data[0] = data[5] = data[10] = data[15] = val;
    }
};

// Basic math functions
inline float length(const vec3& v) { return sqrt(v.x*v.x + v.y*v.y + v.z*v.z); }
inline vec3 normalize(const vec3& v) { float len = length(v); return vec3(v.x/len, v.y/len, v.z/len); }
inline vec3 cross(const vec3& a, const vec3& b) { return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }
inline float radians(float degrees) { return degrees * 3.14159265359f / 180.0f; }

namespace glm {
    using ::vec3;
    using ::vec2;
    using ::mat4;
    using ::length;
    using ::normalize;
    using ::cross;
    using ::radians;
}
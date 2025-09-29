#include "rendering/Collider.hpp"
#include <glm/geometric.hpp>

// Constructors
SphereCollider::SphereCollider()
    : position(0.0f, 0.0f, 0.0f), radius(1.0f) {}

SphereCollider::SphereCollider(const glm::vec3& pos, float r)
    : position(pos), radius(r) {}

// Getters
const glm::vec3& SphereCollider::getPosition() const {
    return position;
}

float SphereCollider::getRadius() const {
    return radius;
}

// Setters
void SphereCollider::setPosition(const glm::vec3& pos) {
    position = pos;
}

void SphereCollider::setRadius(float r) {
    radius = r;
}

void SphereCollider::updatePosition(const glm::vec3& offset) {
    position += offset;
}

// Collision detection
bool SphereCollider::isColliding(const SphereCollider& other) const {
    float distance = getDistanceTo(other);
    return distance <= (radius + other.radius);
}

float SphereCollider::getDistanceTo(const SphereCollider& other) const {
    return glm::length(position - other.position);
}
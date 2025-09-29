#pragma once
#include <glm/vec3.hpp>

class SphereCollider {
public:
    // Constructors
    SphereCollider();
    SphereCollider(const glm::vec3& pos, float r);

    // Getters
    const glm::vec3& getPosition() const;
    float getRadius() const;

    // Setters
    void setPosition(const glm::vec3& pos);
    void setRadius(float r);
    void updatePosition(const glm::vec3& offset);

    // Collision detection
    bool isColliding(const SphereCollider& other) const;
    float getDistanceTo(const SphereCollider& other) const;

private:
    glm::vec3 position;
    float radius;
};

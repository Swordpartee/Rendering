#pragma once

#include "Vector3.hpp"
#include "Component.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/ColliderComponent.hpp"

namespace ParteeEngine {
    struct TransformComponent : public Component {
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;

        void translate(const Vector3 &delta) {
            position.x += delta.x;
            position.y += delta.y;
            position.z += delta.z;
        };
        void setPosition(const Vector3 &delta) {
            position.x = delta.x;
            position.y = delta.y;
            position.z = delta.z;
        };
        void translate(const float x, const float y, const float z)
        {
            position.x += x;
            position.y += y;
            position.z += z;
        };
        void setPosition(const float x, const float y, const float z)
        {
            position.x = x;
            position.y = y;
            position.z = z;
        };
        Vector3& getPosition() {
            return position;
        }

        void rotate(const Vector3 &delta) {
            rotation.x += delta.x;
            rotation.y += delta.y;
            rotation.z += delta.z;
        };
        void setRotation(const Vector3 &delta) {
            rotation.x = delta.x;
            rotation.y = delta.y;
            rotation.z = delta.z;
        };
        void rotate(const float x, const float y, const float z)
        {
            rotation.x += x;
            rotation.y += y;
            rotation.z += z;
        };
        void setRotation(const float x, const float y, const float z)
        {
            rotation.x = x;
            rotation.y = y;
            rotation.z = z;
        };
        Vector3& getRotation() {
            return rotation;
        }

        void addScale(const Vector3 &delta) {
            scale.x += delta.x;
            scale.y += delta.y;
            scale.z += delta.z;
        };
        void setScale(const Vector3 &delta) {
            scale.x = delta.x;
            scale.y = delta.y;
            scale.z = delta.z;
        };
        void addScale(const float x, const float y, const float z)
        {
            scale.x += x;
            scale.y += y;
            scale.z += z;
        };
        void setScale(const float x, const float y, const float z)
        {
            scale.x = x;
            scale.y = y;
            scale.z = z;
        };
        Vector3& getScale() {
            return scale;
        }

        TransformComponent() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

        void update(Entity& owner, float dt) override {
        }
    };
}
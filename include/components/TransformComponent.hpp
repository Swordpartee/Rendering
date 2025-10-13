#include "Vector3.hpp"
#include "Component.hpp"

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

        TransformComponent() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}
    };
}
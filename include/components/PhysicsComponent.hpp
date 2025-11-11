#pragma once

#include "Component.hpp"
#include "Vector3.hpp"
#include "events/Event.hpp"
#include <iostream>
#include <typeindex>

namespace ParteeEngine
{
    class Entity;             // Forward declaration
    class TransformComponent; // Forward declaration

    class PhysicsComponent : public Component {
        public:
            void requireDependencies(Entity &owner) override;

            void update(Entity& owner, float dt) override;

            void applyForce(const Vector3 &force);

            void applyImpulse(const Vector3 &impulse);

            void resetAcceleration();

            void onCollide(CollisionEvent e);

            PhysicsComponent() : velocity(0.0f, 0.0f, 0.0f), acceleration(0.0f, 0.0f, 0.0f) {}

        private:
            Vector3 velocity;
            Vector3 acceleration;
    };
}
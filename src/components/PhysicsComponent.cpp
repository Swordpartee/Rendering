#include "components/TransformComponent.hpp"
#include "Entity.hpp"

#include "components/PhysicsComponent.hpp"
#include "events/Event.hpp"

namespace ParteeEngine
{
    void PhysicsComponent::requireDependencies(Entity &owner) {
        owner.ensureComponent<TransformComponent>();
    }

    std::vector<std::type_index> PhysicsComponent::getUpdateDependencies() const {
        return {};
    }

    void PhysicsComponent::update(Entity& owner, float dt)
    {
        velocity += acceleration * dt;
        owner.getComponent<TransformComponent>()->translate(velocity * dt);
    }

    void PhysicsComponent::applyForce(const Vector3 &force) {
        acceleration += force;
    }

    void PhysicsComponent::applyImpulse(const Vector3 &impulse) {
        velocity += impulse;
    }

    void PhysicsComponent::resetAcceleration() {
        acceleration = Vector3(0.0f, 0.0f, 0.0f);
    }

    void PhysicsComponent::onCollide(CollisionEvent e) {
        acceleration *= -1;
    }
}
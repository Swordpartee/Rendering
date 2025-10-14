#include "components/TransformComponent.hpp"
#include "Entity.hpp"

#include "components/PhysicsComponent.hpp"

namespace ParteeEngine
{
    void PhysicsComponent::requireDependencies(Entity &owner) {
        if (!owner.hasComponent<TransformComponent>()) {
            owner.addComponent<TransformComponent>();
        }
    }

    std::vector<std::type_index> PhysicsComponent::getUpdateDependencies() const {
        return {};
    }

    void PhysicsComponent::update(Entity& owner, float dt)
    {
        velocity = velocity + acceleration * dt;
        owner.getComponent<TransformComponent>()->translate(velocity * dt);
    }

    void PhysicsComponent::applyForce(const Vector3 &force) {
        acceleration = acceleration + force;
    }

    void PhysicsComponent::applyImpulse(const Vector3 &impulse) {
        velocity = velocity + impulse;
    }

    void PhysicsComponent::resetAcceleration() {
        acceleration = Vector3(0.0f, 0.0f, 0.0f);
    }


}
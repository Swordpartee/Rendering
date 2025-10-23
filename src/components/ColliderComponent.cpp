#include "components/ColliderComponent.hpp"

#include "Entity.hpp"
#include "components/TransformComponent.hpp"
#include "events/EventBus.hpp"

namespace ParteeEngine 
{
    void ColliderComponent::requireDependencies(Entity& owner)
    {
        owner.ensureComponent<PhysicsComponent>();
    }

    std::vector<std::type_index> ColliderComponent::getUpdateDependencies() const 
    {
        return {typeid(PhysicsComponent)};
    };

    void ColliderComponent::update(Entity &owner, float dt) 
    {
        if (owner.getComponent<TransformComponent>()->getPosition().x > 0)
        {
            EventBus::instance().emit<CollisionEvent>(CollisionEvent(owner));
        }
    };
}
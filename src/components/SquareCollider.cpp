#include "components/ColliderComponent.hpp"

#include "Entity.hpp"
#include "components/TransformComponent.hpp"
#include "events/EventBus.hpp"

namespace ParteeEngine 
{
    void ColliderComponent::requireDependencies(Entity& owner)
    {
        owner.ensureComponent<TransformComponent>();
    }

    // ColliderComponent::ColliderType ColliderComponent::getColliderType() const 
    // {
    //     return ColliderType::SQUARE;
    // }
}
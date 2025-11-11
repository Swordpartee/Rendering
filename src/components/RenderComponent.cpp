#include "components/RenderComponent.hpp"

#include "components/TransformComponent.hpp"
#include "components/ColliderComponent.hpp"
#include "events/EventBus.hpp"
#include "Renderer.hpp"
#include "Entity.hpp"

namespace ParteeEngine {
    void RenderComponent::requireDependencies(Entity &owner) 
    {
        owner.ensureComponent<TransformComponent>();
    }

    void RenderComponent::render(Entity& owner, Renderer& renderer) 
    {
        if (!visible) return;
        
        auto transform = owner.getComponent<TransformComponent>();
        if (!transform) return;
        
        const Vector3& pos = transform->getPosition();
        
        // Use high-level renderer API
        switch (type) {
            case SQUARE:
                renderer.drawSquare(pos, 1.0f);
                break;
            case CUBE:
                renderer.drawCube(pos, Vector3(1.0f, 1.0f, 1.0f));
                break;
        }
    }
}
#include "components/RenderComponent.hpp"
#include "components/TransformComponent.hpp"
#include "Entity.hpp"

namespace ParteeEngine {
    void RenderComponent::requireDependencies(Entity &owner) {
        if (!owner.hasComponent<TransformComponent>()) {
            owner.addComponent<TransformComponent>();
        }
    }

    std::vector<std::type_index> RenderComponent::getUpdateDependencies() const {
        return {typeid(TransformComponent)};
    }

    void RenderComponent::render() {
        // Rendering code will go here
    }
}
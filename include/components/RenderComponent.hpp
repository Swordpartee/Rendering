#pragma once

#include "Component.hpp"
#include <iostream>
#include <typeindex>

namespace ParteeEngine {
    class Entity; // Forward declaration
    class TransformComponent; // Forward declaration
    
    class RenderComponent : public Component {
        public:
            void requireDependencies(Entity &owner) override;

            std::vector<std::type_index> getUpdateDependencies() const override;

            void render();
    };
}
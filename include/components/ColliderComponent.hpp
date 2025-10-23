#pragma once

#include "Component.hpp"

namespace ParteeEngine 
{
    class Entity;

    class ColliderComponent : public Component 
    {
        public:
            void requireDependencies(Entity &owner) override;

            std::vector<std::type_index> getUpdateDependencies() const override;

            void update(Entity &owner, float dt) override;
    };

} //namespace ParteeEngine
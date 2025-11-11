#pragma once

#include "Component.hpp"

namespace ParteeEngine 
{
    class Entity;

    class ColliderComponent : public Component 
    {
        public:

            void requireDependencies(Entity &owner) override;

    };

} //namespace ParteeEngine
#pragma once
#include <memory>
#include <typeindex>
#include <vector>

namespace ParteeEngine {

    class Entity; // Forward declaration
    class Event;

    class Component {
        public:
            virtual ~Component() = default;
            virtual void onAttach(Entity &owner) {}

            virtual void requireDependencies(Entity&) {}

            virtual void update(Entity& owner, float dt) {}
    };
} // namespace ParteeEngine
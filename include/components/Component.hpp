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

            virtual std::vector<std::type_index> getUpdateDependencies() const { return {}; }

            virtual void update(Entity& owner, float dt) {}
            virtual void onEvent(const Event&) {}
    };
} // namespace ParteeEngine
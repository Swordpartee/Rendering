#pragma once
#include <typeindex>

namespace ParteeEngine {
    class Entity;

    struct Event {
        virtual ~Event() = default;
    };

    struct CollisionEvent : public Event{
        public:
            CollisionEvent(const Entity& first, const Entity& second) : first_(first), second_(second) {}
            const Entity& getFirstEntity() { return first_; }
            const Entity& getSecondEntity() { return second_; }

        private:
            const Entity& first_;
            const Entity& second_;
    };
}
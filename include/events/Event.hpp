#pragma once
#include <typeindex>

namespace ParteeEngine {
    class Entity;

    struct Event {
        virtual ~Event() = default;
    };

    struct CollisionEvent : public Event{
        public:
            CollisionEvent(const Entity& other) : other_(other) {}
            const Entity& getOtherEntity() { return other_; }

        private:
            const Entity& other_;
    };
}
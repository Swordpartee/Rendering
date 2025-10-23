#pragma once
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <memory>
#include <vector>
#include <stdexcept>
#include <functional>

#include "components/Component.hpp"

namespace ParteeEngine {

    class Entity {

        public:
            // Default constructor
            Entity() = default;
            
            // Delete copy constructor and copy assignment
            Entity(const Entity&) = delete;
            Entity& operator=(const Entity&) = delete;

            // Provide move constructor and move assignment
            Entity(Entity &&other) noexcept;

            Entity &operator=(Entity &&other) noexcept;

            template <typename T, typename... Args>
            T &addComponent(Args &&...args);

            template<typename T>
            T* getComponent();

            template<typename T>
            bool hasComponent();

            template <typename T>
            void ensureComponent();

            std::vector<Component*> getComponents() const;

            void update(float dt);

        private:
            bool componentsDirty = true;

            std::unordered_map<std::type_index, std::unique_ptr<Component>> components_;
            std::vector<Component*> sortedComponents_;

            void sortComponents();
            Component* getComponentByType(std::type_index type);
    };

    template <typename T, typename... Args>
    T& Entity::addComponent(Args&&... args)
    {
        // get the component type
        auto type = std::type_index(typeid(T));
        // check if component already exists
        if (components_.find(type) != components_.end()) {
            throw std::runtime_error("Component already exists on this entity");
        }

        // create the component
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->requireDependencies(*this);
        component->onAttach(*this);

        // add the component
        T& ref = *component;
        components_[type] = std::move(component);

        componentsDirty = true;

        return ref;
    }

    template <typename T>
    T* Entity::getComponent()
    {
        auto it = components_.find(std::type_index(typeid(T)));
        return it != components_.end() ? static_cast<T*>(it->second.get()) : nullptr;
    }

    template <typename T>
    bool Entity::hasComponent()
    {
        return components_.count(std::type_index(typeid(T))) > 0;
    }

    template <typename T>
    void Entity::ensureComponent() 
    {
        if (!hasComponent<T>()) 
        {
            addComponent<T>();
        }
    }
}
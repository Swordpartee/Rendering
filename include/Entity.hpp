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
            Entity(Entity&& other) noexcept 
                : componentsDirty(other.componentsDirty),
                  components_(std::move(other.components_)),
                  sortedComponents_(std::move(other.sortedComponents_)) 
            {
                other.componentsDirty = true;
            }
            
            Entity& operator=(Entity&& other) noexcept {
                if (this != &other) {
                    componentsDirty = other.componentsDirty;
                    components_ = std::move(other.components_);
                    sortedComponents_ = std::move(other.sortedComponents_);
                    other.componentsDirty = true;
                }
                return *this;
            }
            
            template <typename T, typename... Args>
            T &addComponent(Args &&...args);

            template<typename T>
            T* getComponent();

            template<typename T>
            bool hasComponent();

            std::vector<Component*> getComponents() const {
                std::vector<Component*> comps;
                for (const auto& pair : components_) {
                    comps.push_back(pair.second.get());
                }
                return comps;
            }

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
        if (components_.count(type))
            throw std::runtime_error("Component Already Exists");

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
}
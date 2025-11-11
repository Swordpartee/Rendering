#include "Entity.hpp"

namespace ParteeEngine {

    Entity::Entity(int id) : id(id) {}

    void Entity::update(float dt) {
        for (auto& pair : components_) {
            pair.second->update(*this, dt);
        }
    }

    // void Entity::sortComponents() {
    //     std::vector<Component*> sorted;
    //     std::unordered_set<std::type_index> visited;

    //     std::function<void(Component*)> visit = [&](Component* c)
    //     {
    //         auto type = std::type_index(typeid(*c));
    //         if (visited.count(type))
    //             return;
    //         visited.insert(type);

    //         for (auto depType : c->getUpdateDependencies())
    //         {
    //             if (auto dep = getComponentByType(depType))
    //                 visit(dep);
    //         }

    //         sorted.push_back(c);
    //     };

    //     for (auto it = components_.begin(); it != components_.end(); ++it) {
    //         visit(it->second.get());
    //     }

    //     sortedComponents_.clear();
    //     for (auto* component : sorted)
    //     {
    //         sortedComponents_.push_back(component);
    //     }
    // }

    Component* Entity::getComponentByType(std::type_index type) {
        auto it = components_.find(type);
        return it != components_.end() ? it->second.get() : nullptr;
    }

    std::vector<Component *> Entity::getComponents() const
    {
        std::vector<Component *> comps;
        for (const auto &pair : components_)
        {
            comps.push_back(pair.second.get());
        }
        return comps;
    };

    int Entity::getID() const
    {
        if (id == -1) {
            throw std::runtime_error("Entity not registered.");
        }
        return id;
    }
}
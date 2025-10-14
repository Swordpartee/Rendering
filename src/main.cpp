#include "Engine.hpp"
#include "Entity.hpp"
#include "components/RenderComponent.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/TransformComponent.hpp"
#include <utility>

int main() 
{   
    ParteeEngine::Engine engine(800, 600);

    ParteeEngine::Entity thingy;

    thingy.addComponent<ParteeEngine::RenderComponent>();

    thingy.addComponent<ParteeEngine::PhysicsComponent>();

    thingy.getComponent<ParteeEngine::TransformComponent>()->setPosition(-3, -2, 0);

    thingy.getComponent<ParteeEngine::PhysicsComponent>()->applyImpulse(ParteeEngine::Vector3(5.0f, 0.0f, 0.0f));
    thingy.getComponent<ParteeEngine::PhysicsComponent>()->applyForce(ParteeEngine::Vector3(0.0f, 30.0f, 0.0f));

    engine.addEntity(std::move(thingy));

    engine.start();

    return 0;
}
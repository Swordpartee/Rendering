#include "Engine.hpp"
#include "Entity.hpp"
#include "components/RenderComponent.hpp"
#include <utility>

int main() 
{   
    ParteeEngine::Engine engine(800, 600);

    ParteeEngine::Entity thingy;

    thingy.addComponent<ParteeEngine::RenderComponent>();

    engine.addEntity(std::move(thingy));

    engine.start();

    return 0;
}
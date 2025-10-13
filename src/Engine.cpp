#include "Engine.hpp"
#include "Window.hpp"
#include "components/RenderComponent.hpp"

namespace ParteeEngine {

    Engine::Engine(int width, int height) : width(width), height(height) {
        window = new Window(width, height);

        window->setRenderCallback([&]() {
            for (Entity& e : entities) {
                e.update(0.0016);

                auto renderComp = e.getComponent<RenderComponent>();
                if (renderComp != nullptr) {
                    renderComp->render();
                } else {
                }
            }
        });
    }

    void Engine::start() {
        window->show();
    }

    void Engine::addEntity(Entity&& entity) {
        entities.push_back(std::move(entity));
    }
}
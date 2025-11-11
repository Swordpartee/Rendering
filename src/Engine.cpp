#include "Engine.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Vector3.hpp"
#include "components/RenderComponent.hpp"
#include "components/PhysicsComponent.hpp"
#include "components/ColliderComponent.hpp"

namespace ParteeEngine {

    Engine::Engine(int width, int height) : width(width), height(height) {
        window = new Window(width, height);
        renderer = new Renderer();
        
        // Initialize the renderer after OpenGL context is created
        renderer->initialize(width, height);

        window->setRenderCallback([&]() {
            static int frameCount = 0;
            frameCount++;
            
            // Clear the screen
            renderer->clear();

            for (Entity &e : entities) { e.updateComponent<PhysicsComponent>(0.0016f); }

            for (Entity &e : entities) { e.updateComponent<ColliderComponent>(0.0016f); }

            // Update and render entities
            for (Entity& e : entities) {
                auto renderComp = e.getComponent<RenderComponent>();
                if (renderComp) renderComp->render(e, *renderer);
            }
            
            // Present the frame
            renderer->present();
        });
    }

    void Engine::start() {
        window->show();
    }

    Entity& Engine::createEntity() {
        int newID = static_cast<int>(entities.size());
        entities.emplace_back(newID);
        return entities.back();
    }
    
    Engine::~Engine() {
        delete renderer;
        delete window;
    }
}
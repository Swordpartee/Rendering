#include "Engine.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Vector3.hpp"
#include "components/RenderComponent.hpp"

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
            
            // Update and render entities
            for (Entity& e : entities) {
                e.update(0.0016f);

                auto renderComp = e.getComponent<RenderComponent>();
                if (renderComp != nullptr) {
                    renderComp->render(e, *renderer);
                }
            }
            
            // Present the frame
            renderer->present();
        });
    }

    void Engine::start() {
        window->show();
    }

    void Engine::addEntity(Entity&& entity) {
        entities.push_back(std::move(entity));
    }
    
    Engine::~Engine() {
        delete renderer;
        delete window;
    }
}
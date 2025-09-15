#include "rendering/ParteeEngine.hpp"
#include "rendering/RenderObject.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <random>

int main() 
{
    // Create and initialize engine
    Rendering::ParteeEngine engine(800, 600);
    engine.initialize();

    // Set up camera further back to see all the cubes
    auto& camera = engine.getCamera();
    camera.setPosition(glm::vec3(0.0f, 10.0f, 100.0f));
    camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

    // Set up random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> positionDist(-50.0f, 50.0f);
    std::uniform_real_distribution<float> scaleDist(0.1f, 2.0f);
    std::uniform_real_distribution<float> rotationDist(-3.14159f, 3.14159f);

    // Create 1000 cubes procedurally
    for (int i = 0; i < 10000; ++i)
    {
        auto cube = std::make_shared<Rendering::CubeObject>();
        
        // Random position in a 100x100x100 cube centered at origin
        glm::vec3 position(
            positionDist(gen),
            positionDist(gen),
            positionDist(gen)
        );
        cube->setPosition(position);
        
        // Random scale between 0.1 and 2.0
        float scale = scaleDist(gen);
        cube->setScale(glm::vec3(scale, scale, scale));
        
        // Random initial rotation
        glm::vec3 rotation(
            rotationDist(gen),
            rotationDist(gen),
            rotationDist(gen)
        );
        cube->setRotation(rotation);
        
        engine.addRenderObject(cube);
    }

    // Run the engine (this will show the window and start the render loop)
    engine.run();

    return 0;
}
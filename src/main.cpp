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
    camera.setPosition(glm::vec3(0.0f, 0.0f, 100.0f));
    camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

    // Set up random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> positionDist(-50.0f, 50.0f);
    std::uniform_real_distribution<float> scaleDist(0.1f, 2.0f);
    std::uniform_real_distribution<float> rotationDist(-3.14159f, 3.14159f);
    std::uniform_real_distribution<float> colorDist(0.2f, 1.0f);  // For random colors

    // Create objects procedurally - just cubes with different colors for now
    for (int i = 0; i < 1000; ++i)
    {
        auto sphere = std::make_shared<Rendering::CubeObject>();

        // Give each sphere a different color based on its index
        float r = (float)(i % 255) / 255.0f;
        float g = (float)((i * 7) % 255) / 255.0f;
        float b = (float)((i * 13) % 255) / 255.0f;
        sphere->setColor(glm::vec3(r, g, b));

        // Random position in a 100x100x100 cube centered at origin
        glm::vec3 position(
            positionDist(gen),
            positionDist(gen),
            positionDist(gen)
        );
        sphere->setPosition(position);

        // Random rotation between 0 and 360 degrees
        glm::vec3 rotation(
            rotationDist(gen),
            rotationDist(gen),
            rotationDist(gen)
        );
        sphere->setRotation(rotation);

        // Random scale between 0.1 and 2.0
        float scale = scaleDist(gen);
        sphere->setScale(glm::vec3(scale, scale, scale));

        engine.addRenderObject(sphere);
    }

    // Run the engine (this will show the window and start the render loop)
    engine.run();

    return 0;
}
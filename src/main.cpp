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

    // Add a pyramid loaded from OBJ file
    auto pyramid = std::make_shared<Rendering::ObjObject>("assets/models/RubberDuck_LOD0.obj");
    pyramid->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    pyramid->setScale(glm::vec3(1.0f, 1.0f, 1.0f)); // Make it bigger
    pyramid->setColor(glm::vec3(1.0f, 0.8f, 0.2f)); // Golden color
    // engine.addRenderObject(pyramid);

    // Set up random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> positionDist(-50.0f, 50.0f);
    std::uniform_real_distribution<float> scaleDist(1.0f, 10.0f);
    std::uniform_real_distribution<float> rotationDist(-3.14159f, 3.14159f);
    std::uniform_real_distribution<float> colorDist(0.2f, 1.0f);  // For random colors
    std::uniform_real_distribution<float> linearVelocityDist(-1.0f, 1.0f);
    std::uniform_real_distribution<float> angularVelocityDist(-1.0f, 1.0f);

    // Create objects procedurally - spheres with collision detection
    for (int i = 0; i < 50; ++i)  // Reduced count for better performance
    {
        float radius = scaleDist(gen) * 10.0f; // Random radius between 0.1 and 1.0
        auto sphere = std::make_shared<Rendering::SphereObject>(16, 16, radius);

        // Give each sphere a different color based on its index
        float r = (float)(i % 255) / 255.0f;
        float g = (float)((i * 7) % 255) / 255.0f;
        float b = (float)((i * 13) % 255) / 255.0f;
        // sphere->setColor(glm::vec3(r, g, b));

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

        glm::vec3 linearVelocity(
            linearVelocityDist(gen) * 1.0f,
            linearVelocityDist(gen) * 1.0f,
            linearVelocityDist(gen) * 1.0f
        );
        sphere->setLinearVelocity(linearVelocity);

        glm::vec3 angularVelocity(
            angularVelocityDist(gen) * 1.0f,
            angularVelocityDist(gen) * 1.0f,
            angularVelocityDist(gen) * 1.0f
        );
        sphere->setAngularVelocity(angularVelocity);

        // Random scale between 0.1 and 2.0
        float scale = scaleDist(gen);
        sphere->setScale(glm::vec3(scale, scale, scale));

        engine.addRenderObject(sphere);
    }

    // Run the engine (this will show the window and start the render loop)
    engine.run();

    return 0;
}
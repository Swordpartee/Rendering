#include "rendering/ParteeEngine.hpp"
#include "rendering/RenderObject.hpp"
#include <glm/glm.hpp>
#include <memory>

int main() 
{
    // Create and initialize engine
    Rendering::ParteeEngine engine(800, 600);
    engine.initialize();

    // Set up camera
    auto& camera = engine.getCamera();
    camera.setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    camera.setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

    // Create a cube at the origin
    auto cube1 = std::make_shared<Rendering::CubeObject>();
    cube1->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    engine.addRenderObject(cube1);

    // Create another cube to the right
    auto cube2 = std::make_shared<Rendering::CubeObject>();
    cube2->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    cube2->setScale(glm::vec3(0.5f, 0.5f, 0.5f));  // Make it smaller
    engine.addRenderObject(cube2);

    // Create a third cube above
    auto cube3 = std::make_shared<Rendering::CubeObject>();
    cube3->setPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    cube3->setScale(glm::vec3(0.8f, 0.8f, 0.8f));
    engine.addRenderObject(cube3);

    // Run the engine (this will show the window and start the render loop)
    engine.run();

    return 0;
}
#include "ParteeWindow.hpp"
#include "ParteeRenderer.hpp"

int main() 
{
    Rendering::ParteeWindow window(800, 600);
    Rendering::ParteeRenderer renderer(window);

    window.setRenderCallback([&renderer]() {
        renderer.render();
    });

    window.show();

    return 0;
}
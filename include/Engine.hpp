#pragma once

#include <vector>

#include "entity.hpp"

namespace ParteeEngine {
    class Window;
    class Renderer; 

    class Engine {

        public:
            Engine(int width, int height);
            ~Engine();

            void start();

            Entity& createEntity();

        private:
            int width;
            int height;

            void update();

            Window* window;
            Renderer* renderer;

            std::vector<Entity> entities;
    };
} // namespace ParteeEngine

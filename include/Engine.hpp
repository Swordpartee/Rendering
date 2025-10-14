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

            void addEntity(Entity&& entity);

        private:
            int width;
            int height;

            void update();

            Window* window;
            Renderer* renderer;

            std::vector<Entity> entities;
    };
} // namespace ParteeEngine

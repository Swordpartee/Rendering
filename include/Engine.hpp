#pragma once

#include <vector>

#include "entity.hpp"

namespace ParteeEngine {
    class Window; 

    class Engine {

        public:
            Engine(int width, int height);
            ~Engine() = default;

            void start();

            void addEntity(Entity&& entity);

        private:
            int width;
            int height;

            void update();

            Window* window;

            std::vector<Entity> entities;
    };
} // namespace ParteeEngine

#pragma once

#include "ParteeWindow.hpp"
#include "ParteeRenderer.hpp"
#include "ParteeCamera.hpp"
#include "RenderObject.hpp"
#include <vector>
#include <memory>
#include <chrono>

namespace Rendering 
{
    class ParteeEngine 
    {
    public:
        ParteeEngine(int windowWidth = 800, int windowHeight = 600);
        ~ParteeEngine();

        // Engine lifecycle
        void initialize();
        void run();
        void shutdown();

        // Object management
        void addRenderObject(std::shared_ptr<RenderObject> object);
        void removeRenderObject(std::shared_ptr<RenderObject> object);
        
        // Collision detection
        void checkCollisions();

        // Camera access
        ParteeCamera& getCamera() { return *m_camera; }
        
        // Getters
        ParteeWindow& getWindow() { return *m_window; }
        ParteeRenderer& getRenderer() { return *m_renderer; }

        // Time management
        float getDeltaTime() const { return m_deltaTime; }
        float getTotalTime() const { return m_totalTime; }

    private:
        std::unique_ptr<ParteeWindow> m_window;
        std::unique_ptr<ParteeRenderer> m_renderer;
        std::unique_ptr<ParteeCamera> m_camera;
        
        std::vector<std::shared_ptr<RenderObject>> m_renderObjects;
        
        bool m_isRunning;
        
        // Timing
        std::chrono::high_resolution_clock::time_point m_lastFrameTime;
        float m_deltaTime;
        float m_totalTime;
        
        void update(float deltaTime);
        void render();
        void calculateDeltaTime();
    };
}
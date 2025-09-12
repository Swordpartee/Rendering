#include "rendering/ParteeEngine.hpp"
#include <algorithm>
#include <iostream>

namespace Rendering 
{
    ParteeEngine::ParteeEngine(int windowWidth, int windowHeight)
        : m_isRunning(false), m_deltaTime(0.0f), m_totalTime(0.0f)
    {
        // Create window
        m_window = std::make_unique<ParteeWindow>(windowWidth, windowHeight);
        
        // Create camera with proper aspect ratio
        float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
        m_camera = std::make_unique<ParteeCamera>(45.0f, aspectRatio, 0.1f, 100.0f);
        
        // Set default camera position
        m_camera->setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        m_camera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    ParteeEngine::~ParteeEngine()
    {
        shutdown();
    }

    void ParteeEngine::initialize()
    {
        // Create renderer after window is created
        m_renderer = std::make_unique<ParteeRenderer>(*m_window);
        
        // Initialize renderer
        m_renderer->initialize();
        
        // Set render callback
        m_window->setRenderCallback([this]() {
            this->render();
        });
        
        // Initialize timing
        m_lastFrameTime = std::chrono::high_resolution_clock::now();
        m_isRunning = true;
    }

    void ParteeEngine::run()
    {
        if (!m_isRunning)
        {
            std::cerr << "Engine not initialized! Call initialize() first." << std::endl;
            return;
        }
        
        // Show window and start main loop
        m_window->show();
        
        // The window's show() method handles the message loop,
        // so we don't need additional loop logic here
    }

    void ParteeEngine::shutdown()
    {
        m_isRunning = false;
        m_renderObjects.clear();
        
        // Renderer, camera, and window will be automatically destroyed
        // by their unique_ptr destructors
    }

    void ParteeEngine::addRenderObject(std::shared_ptr<RenderObject> object)
    {
        if (object)
        {
            m_renderObjects.push_back(object);
        }
    }

    void ParteeEngine::removeRenderObject(std::shared_ptr<RenderObject> object)
    {
        auto it = std::find(m_renderObjects.begin(), m_renderObjects.end(), object);
        if (it != m_renderObjects.end())
        {
            m_renderObjects.erase(it);
        }
    }

    void ParteeEngine::update(float deltaTime)
    {
        // Update total time
        m_totalTime += deltaTime;
        
        // Example: Rotate all objects slightly
        for (auto& object : m_renderObjects)
        {
            if (object)
            {
                object->rotate(deltaTime * 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
            }
        }
        
        // Here you could add:
        // - Input handling
        // - Physics updates
        // - Animation updates
        // - Game logic
    }

    void ParteeEngine::render()
    {
        if (!m_isRunning || !m_renderer)
            return;
            
        calculateDeltaTime();
        update(m_deltaTime);
        
        // Clear the screen
        m_renderer->clear();
        
        // Render all objects
        m_renderer->renderScene(m_renderObjects, *m_camera);
        
        // Present the frame
        m_renderer->present();
    }

    void ParteeEngine::calculateDeltaTime()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_lastFrameTime);
        m_deltaTime = duration.count() / 1000000.0f; // Convert to seconds
        m_lastFrameTime = currentTime;
        
        // Cap delta time to prevent large jumps (e.g., when debugging)
        if (m_deltaTime > 0.1f)
        {
            m_deltaTime = 0.016f; // ~60 FPS
        }
    }
}
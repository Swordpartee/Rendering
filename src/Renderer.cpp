#include "Renderer.hpp"
#include <iostream>

namespace ParteeEngine {

    Renderer::Renderer() : renderContext(std::make_unique<RenderContext>()) {
        std::cout << "Renderer created" << std::endl;
    }
    
    Renderer::~Renderer() {
        std::cout << "Renderer destroyed" << std::endl;
    }
    
    void Renderer::initialize(int width, int height) {
        renderContext->initialize(width, height);
        std::cout << "Renderer initialized with RenderContext" << std::endl;
    }
    
    void Renderer::clear() {
        renderContext->clear();
    }
    
    void Renderer::present() {
        renderContext->present();
    }

    void Renderer::drawSquare(const Vector3& position, float size) {
        float halfSize = size * 0.5f;
        
        // Draw two triangles to form a square
        renderContext->drawTriangle(
            Vector3(position.x - halfSize, position.y - halfSize, position.z),
            Vector3(position.x + halfSize, position.y - halfSize, position.z),
            Vector3(position.x - halfSize, position.y + halfSize, position.z)
        );
        renderContext->drawTriangle(
            Vector3(position.x + halfSize, position.y - halfSize, position.z),
            Vector3(position.x + halfSize, position.y + halfSize, position.z),
            Vector3(position.x - halfSize, position.y + halfSize, position.z)
        );
    }

    void Renderer::drawTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3) {
        renderContext->drawTriangle(v1, v2, v3);
    }

    void Renderer::drawCube(const Vector3& position, const Vector3& size) {
        renderContext->pushMatrix();
        renderContext->translate(position);
        renderContext->scale(size);
        
        // Draw cube using RenderContext
        renderContext->beginQuads();
        
        // Front face
        renderContext->color(1.0f, 0.0f, 0.0f);
        renderContext->vertex(Vector3(-0.5f, -0.5f,  0.5f));
        renderContext->vertex(Vector3( 0.5f, -0.5f,  0.5f));
        renderContext->vertex(Vector3( 0.5f,  0.5f,  0.5f));
        renderContext->vertex(Vector3(-0.5f,  0.5f,  0.5f));
        
        // Back face
        renderContext->color(0.0f, 1.0f, 0.0f);
        renderContext->vertex(Vector3(-0.5f, -0.5f, -0.5f));
        renderContext->vertex(Vector3(-0.5f,  0.5f, -0.5f));
        renderContext->vertex(Vector3( 0.5f,  0.5f, -0.5f));
        renderContext->vertex(Vector3( 0.5f, -0.5f, -0.5f));
        
        // Top face
        renderContext->color(0.0f, 0.0f, 1.0f);
        renderContext->vertex(Vector3(-0.5f,  0.5f, -0.5f));
        renderContext->vertex(Vector3(-0.5f,  0.5f,  0.5f));
        renderContext->vertex(Vector3( 0.5f,  0.5f,  0.5f));
        renderContext->vertex(Vector3( 0.5f,  0.5f, -0.5f));
        
        // Bottom face
        renderContext->color(1.0f, 1.0f, 0.0f);
        renderContext->vertex(Vector3(-0.5f, -0.5f, -0.5f));
        renderContext->vertex(Vector3( 0.5f, -0.5f, -0.5f));
        renderContext->vertex(Vector3( 0.5f, -0.5f,  0.5f));
        renderContext->vertex(Vector3(-0.5f, -0.5f,  0.5f));
        
        // Right face
        renderContext->color(1.0f, 0.0f, 1.0f);
        renderContext->vertex(Vector3( 0.5f, -0.5f, -0.5f));
        renderContext->vertex(Vector3( 0.5f,  0.5f, -0.5f));
        renderContext->vertex(Vector3( 0.5f,  0.5f,  0.5f));
        renderContext->vertex(Vector3( 0.5f, -0.5f,  0.5f));
        
        // Left face
        renderContext->color(0.0f, 1.0f, 1.0f);
        renderContext->vertex(Vector3(-0.5f, -0.5f, -0.5f));
        renderContext->vertex(Vector3(-0.5f, -0.5f,  0.5f));
        renderContext->vertex(Vector3(-0.5f,  0.5f,  0.5f));
        renderContext->vertex(Vector3(-0.5f,  0.5f, -0.5f));
        
        renderContext->end();
        renderContext->popMatrix();
    }
    
    void Renderer::setCamera(const Vector3& position, const Vector3& target, const Vector3& up) {
        renderContext->setCamera(position, target, up);
    }
    
    void Renderer::setPerspective(float fov, float aspect, float nearPlane, float farPlane) {
        renderContext->setPerspective(fov, aspect, nearPlane, farPlane);
    }
    
}
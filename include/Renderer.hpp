#pragma once

#include "RenderContext.hpp"
#include "Vector3.hpp"
#include <memory>

namespace ParteeEngine {

    class Renderer {
    public:
        Renderer();
        ~Renderer();
        
        void initialize(int width, int height);
        void clear();
        void present();
        
        // High-level drawing functions
        void drawSquare(const Vector3& position, float size = 1.0f);
        void drawCube(const Vector3& position, const Vector3& size);
        void drawTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        
        // Camera operations
        void setCamera(const Vector3& position, const Vector3& target, const Vector3& up);
        void setPerspective(float fov, float aspect, float near, float far);
        
        // Get render context for low-level operations (use sparingly)
        RenderContext& getRenderContext() { return *renderContext; }
        
    private:
        std::unique_ptr<RenderContext> renderContext;
        void loadMatrix(const Matrix4& matrix);
    };
}
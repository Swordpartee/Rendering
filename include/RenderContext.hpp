#pragma once

#include <windows.h>
#include <GL/gl.h>
#include "Vector3.hpp"

namespace ParteeEngine {

    struct Matrix4;

    class RenderContext {
    public:
        RenderContext();
        ~RenderContext();

        // Context management
        void initialize(int width, int height);
        void clear();
        void present();

        // Transformation matrix operations
        void pushMatrix();
        void popMatrix();
        void loadIdentity();
        void translate(const Vector3& position);
        void rotate(const Vector3& rotation);
        void scale(const Vector3& scale);
        void multMatrix(const Matrix4& matrix);

        // Drawing operations
        void setColor(float r, float g, float b, float a = 1.0f);
        void drawTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3);
        void drawQuad(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4);
        
        // Immediate mode helpers
        void beginTriangles();
        void beginQuads();
        void end();
        void vertex(const Vector3& v);
        void color(float r, float g, float b, float a = 1.0f);

        // State management
        void enableDepthTest(bool enable);
        void setCullFace(bool enable);
        void setViewport(int x, int y, int width, int height);

        // Camera and projection
        void setPerspective(float fov, float aspect, float near, float far);
        void setCamera(const Vector3& position, const Vector3& target, const Vector3& up);

    private:
        int viewportWidth;
        int viewportHeight;
        bool initialized;

        void setupPerspective();
        void setupCamera();
        
        Vector3 cameraPosition;
        Vector3 cameraTarget;
        Vector3 cameraUp;
        float fov;
        float aspect;
        float nearPlane;
        float farPlane;
    };

}
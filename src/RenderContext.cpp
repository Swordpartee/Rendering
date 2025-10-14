#include "RenderContext.hpp"
#include <cmath>
#include <iostream>

namespace ParteeEngine {

    RenderContext::RenderContext() 
        : viewportWidth(800), viewportHeight(600), initialized(false),
          cameraPosition(0, 0, 10), cameraTarget(0, 0, 0), cameraUp(0, 1, 0),
          fov(45.0f), aspect(4.0f/3.0f), nearPlane(0.1f), farPlane(100.0f) {
    }

    RenderContext::~RenderContext() {
        // Cleanup handled by Window class
    }

    void RenderContext::initialize(int width, int height) {
        viewportWidth = width;
        viewportHeight = height;
        aspect = static_cast<float>(width) / static_cast<float>(height);

        // Set up OpenGL state
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        // Clear color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        setViewport(0, 0, width, height);
        setPerspective(fov, aspect, nearPlane, farPlane);
        
        initialized = true;
        std::cout << "RenderContext initialized: " << width << "x" << height << std::endl;
    }

    void RenderContext::clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Reset matrices
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        setupPerspective();
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        setupCamera();
    }

    void RenderContext::present() {
        glFlush();
        // Note: SwapBuffers is handled by Window class
    }

    void RenderContext::pushMatrix() {
        glPushMatrix();
    }

    void RenderContext::popMatrix() {
        glPopMatrix();
    }

    void RenderContext::loadIdentity() {
        glLoadIdentity();
    }

    void RenderContext::translate(const Vector3& position) {
        glTranslatef(position.x, position.y, position.z);
    }

    void RenderContext::rotate(const Vector3& rotation) {
        // Apply rotations in order: Y, X, Z (commonly used for Euler angles)
        glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);  // Yaw
        glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);  // Pitch
        glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);  // Roll
    }

    void RenderContext::scale(const Vector3& scale) {
        glScalef(scale.x, scale.y, scale.z);
    }

    void RenderContext::setColor(float r, float g, float b, float a) {
        glColor4f(r, g, b, a);
    }

    void RenderContext::drawTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3) {
        glBegin(GL_TRIANGLES);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }

    void RenderContext::drawQuad(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4) {
        glBegin(GL_QUADS);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glVertex3f(v4.x, v4.y, v4.z);
        glEnd();
    }

    void RenderContext::beginTriangles() {
        glBegin(GL_TRIANGLES);
    }

    void RenderContext::beginQuads() {
        glBegin(GL_QUADS);
    }

    void RenderContext::end() {
        glEnd();
    }

    void RenderContext::vertex(const Vector3& v) {
        glVertex3f(v.x, v.y, v.z);
    }

    void RenderContext::color(float r, float g, float b, float a) {
        glColor4f(r, g, b, a);
    }

    void RenderContext::enableDepthTest(bool enable) {
        if (enable) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void RenderContext::setCullFace(bool enable) {
        if (enable) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    void RenderContext::setViewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
        viewportWidth = width;
        viewportHeight = height;
        aspect = static_cast<float>(width) / static_cast<float>(height);
    }

    void RenderContext::setPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane) {
        fov = fovDegrees;
        aspect = aspectRatio;
        nearPlane = nearPlane;
        farPlane = farPlane;
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        setupPerspective();
        glMatrixMode(GL_MODELVIEW);
    }

    void RenderContext::setCamera(const Vector3& position, const Vector3& target, const Vector3& up) {
        cameraPosition = position;
        cameraTarget = target;
        cameraUp = up;
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        setupCamera();
    }

    void RenderContext::setupPerspective() {
        // Manual perspective projection matrix setup
        float fovRadians = fov * 3.14159265359f / 180.0f;
        float f = 1.0f / tan(fovRadians / 2.0f);
        
        // Set up perspective matrix manually using glFrustum
        float ymax = nearPlane * tan(fovRadians / 2.0f);
        float ymin = -ymax;
        float xmin = ymin * aspect;
        float xmax = ymax * aspect;
        
        glFrustum(xmin, xmax, ymin, ymax, nearPlane, farPlane);
    }

    void RenderContext::setupCamera() {
        // Manual lookAt implementation
        Vector3 forward = (cameraTarget - cameraPosition).normalize();
        Vector3 right = forward.cross(cameraUp).normalize();
        Vector3 up = right.cross(forward);
        
        // Create lookAt matrix manually
        float matrix[16] = {
            right.x, up.x, -forward.x, 0,
            right.y, up.y, -forward.y, 0,
            right.z, up.z, -forward.z, 0,
            -right.dot(cameraPosition), -up.dot(cameraPosition), forward.dot(cameraPosition), 1
        };
        
        glMultMatrixf(matrix);
    }

}
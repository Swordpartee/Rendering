#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Rendering 
{
    class ParteeCamera 
    {
    public:
        ParteeCamera(float fov = 45.0f, float aspectRatio = 16.0f/9.0f, 
                     float nearPlane = 0.1f, float farPlane = 100.0f);

        // Matrix getters
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;
        glm::mat4 getViewProjectionMatrix() const;

        // Position and orientation
        void setPosition(const glm::vec3& position);
        void setTarget(const glm::vec3& target);
        void setUpVector(const glm::vec3& up);
        
        glm::vec3 getPosition() const { return m_position; }
        glm::vec3 getTarget() const { return m_target; }
        glm::vec3 getForward() const { return glm::normalize(m_target - m_position); }

        // Projection settings
        void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
        void setOrthographic(float left, float right, float bottom, float top, 
                           float nearPlane, float farPlane);

        // Camera movement
        void moveForward(float distance);
        void moveRight(float distance);
        void moveUp(float distance);
        void rotate(float yaw, float pitch);

        // Utility
        void lookAt(const glm::vec3& position, const glm::vec3& target, 
                   const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    private:
        // Position and orientation
        glm::vec3 m_position;
        glm::vec3 m_target;
        glm::vec3 m_up;

        // Projection parameters
        float m_fov;
        float m_aspectRatio;
        float m_nearPlane;
        float m_farPlane;
        
        bool m_isPerspective;
        
        // Cached matrices (marked mutable for lazy evaluation)
        mutable glm::mat4 m_viewMatrix;
        mutable glm::mat4 m_projectionMatrix;
        mutable bool m_viewMatrixDirty;
        mutable bool m_projectionMatrixDirty;

        void updateViewMatrix() const;
        void updateProjectionMatrix() const;
    };
}
#include "rendering/ParteeCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Rendering 
{
    ParteeCamera::ParteeCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
        : m_fov(fov), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane),
          m_position(0.0f, 0.0f, 3.0f), m_target(0.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f),
          m_isPerspective(true), m_viewMatrixDirty(true), m_projectionMatrixDirty(true)
    {
    }

    glm::mat4 ParteeCamera::getViewMatrix() const
    {
        if (m_viewMatrixDirty)
        {
            updateViewMatrix();
        }
        return m_viewMatrix;
    }

    glm::mat4 ParteeCamera::getProjectionMatrix() const
    {
        if (m_projectionMatrixDirty)
        {
            updateProjectionMatrix();
        }
        return m_projectionMatrix;
    }

    glm::mat4 ParteeCamera::getViewProjectionMatrix() const
    {
        return getProjectionMatrix() * getViewMatrix();
    }

    void ParteeCamera::setPosition(const glm::vec3& position)
    {
        m_position = position;
        m_viewMatrixDirty = true;
    }

    void ParteeCamera::setTarget(const glm::vec3& target)
    {
        m_target = target;
        m_viewMatrixDirty = true;
    }

    void ParteeCamera::setUpVector(const glm::vec3& up)
    {
        m_up = up;
        m_viewMatrixDirty = true;
    }

    void ParteeCamera::setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        m_fov = fov;
        m_aspectRatio = aspectRatio;
        m_nearPlane = nearPlane;
        m_farPlane = farPlane;
        m_isPerspective = true;
        m_projectionMatrixDirty = true;
    }

    void ParteeCamera::setOrthographic(float left, float right, float bottom, float top, 
                                     float nearPlane, float farPlane)
    {
        m_nearPlane = nearPlane;
        m_farPlane = farPlane;
        m_isPerspective = false;
        m_projectionMatrixDirty = true;
    }

    void ParteeCamera::moveForward(float distance)
    {
        glm::vec3 forward = getForward();
        m_position += forward * distance;
        m_target += forward * distance;
        m_viewMatrixDirty = true;
    }

    void ParteeCamera::moveRight(float distance)
    {
        glm::vec3 forward = getForward();
        glm::vec3 right = glm::normalize(glm::cross(forward, m_up));
        m_position += right * distance;
        m_target += right * distance;
        m_viewMatrixDirty = true;
    }

    void ParteeCamera::moveUp(float distance)
    {
        m_position += m_up * distance;
        m_target += m_up * distance;
        m_viewMatrixDirty = true;
    }

    void ParteeCamera::rotate(float yaw, float pitch)
    {
        // Calculate the direction vector from position to target
        glm::vec3 direction = m_target - m_position;
        float distance = glm::length(direction);
        
        // Convert to spherical coordinates, apply rotation, convert back
        float currentYaw = atan2(direction.z, direction.x);
        float currentPitch = asin(direction.y / distance);
        
        currentYaw += yaw;
        currentPitch += pitch;
        
        // Clamp pitch to avoid gimbal lock
        currentPitch = glm::clamp(currentPitch, -1.5f, 1.5f);
        
        // Calculate new target position
        direction.x = cos(currentPitch) * cos(currentYaw);
        direction.y = sin(currentPitch);
        direction.z = cos(currentPitch) * sin(currentYaw);
        
        m_target = m_position + glm::normalize(direction) * distance;
        m_viewMatrixDirty = true;
    }

    void ParteeCamera::lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
    {
        m_position = position;
        m_target = target;
        m_up = up;
        m_viewMatrixDirty = true;
    }

    void ParteeCamera::updateViewMatrix() const
    {
        m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
        m_viewMatrixDirty = false;
    }

    void ParteeCamera::updateProjectionMatrix() const
    {
        if (m_isPerspective)
        {
            m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
        }
        else
        {
            // For orthographic, we need to store the bounds
            // This is a simplified version - you might want to add proper ortho bounds storage
            float halfWidth = 5.0f;
            float halfHeight = halfWidth / m_aspectRatio;
            m_projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_nearPlane, m_farPlane);
        }
        m_projectionMatrixDirty = false;
    }
}